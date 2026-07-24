import React, { useState } from 'react';
import {
  View,
  Text,
  TextInput,
  TouchableOpacity,
  ScrollView,
  StyleSheet,
  Alert,
  ActivityIndicator,
} from 'react-native';
import { StackNavigationProp } from '@react-navigation/stack';

import {
  defaultProducts,
  addProduct,
  validateProduct,
} from '../../services/productService';

type RootStackParamList = {
  TambahProduk: undefined;
};
type NavigationProp = StackNavigationProp<RootStackParamList, 'TambahProduk'>;

interface Props {
  navigation: NavigationProp;
}

interface FormProduct {
  id: string;
  name: string;
  price: string;
  stock: string;
  category: string;
}

const TambahProdukScreen: React.FC<Props> = ({ navigation }) => {
  // State produk – diisi dari defaultProducts
  const [products, setProducts] = useState<FormProduct[]>(
    defaultProducts.map((item, index) => ({
      id: Date.now().toString() + index,
      name: item.name,
      price: item.price.toString(),
      stock: item.stock.toString(),
      category: item.category,
    }))
  );

  const [loading, setLoading] = useState<boolean>(false);

  // Tambah baris kosong
  const addRow = (): void => {
    const newProduct: FormProduct = {
      id: Date.now().toString() + Math.random(),
      name: '',
      price: '',
      stock: '',
      category: '',
    };
    setProducts([...products, newProduct]);
  };

  // Hapus baris
  const removeRow = (id: string): void => {
    if (products.length <= 1) {
      Alert.alert('Info', 'Minimal harus ada 1 produk.');
      return;
    }
    setProducts(products.filter((item) => item.id !== id));
  };

  // Update field
  const updateField = (id: string, field: keyof FormProduct, value: string): void => {
    setProducts(
      products.map((item) => (item.id === id ? { ...item, [field]: value } : item))
    );
  };

  // Simpan semua produk
  const handleSubmit = async (): Promise<void> => {
    // Validasi field kosong
    const isEmpty = products.some(
      (item) =>
        item.name.trim() === '' ||
        item.price.trim() === '' ||
        item.stock.trim() === '' ||
        item.category.trim() === ''
    );

    if (isEmpty) {
      Alert.alert('Error', 'Semua field harus diisi!');
      return;
    }

    setLoading(true);

    try {
      const addPromises = products.map(async (item) => {
        const productData = {
          name: item.name.trim(),
          price: parseInt(item.price, 10),
          stock: parseInt(item.stock, 10),
          category: item.category.trim(),
        };

        const validation = validateProduct(productData);
        if (Object.keys(validation).length > 0) {
          throw new Error(
            `Validasi gagal untuk "${item.name}": ${JSON.stringify(validation)}`
          );
        }

        await addProduct(productData);
      });

      await Promise.all(addPromises);

      Alert.alert(
        'Sukses',
        `Berhasil menyimpan ${products.length} data produk ke Firebase!`,
        [
          {
            text: 'OK',
            onPress: () => {
              // Reset form ke default
              setProducts(
                defaultProducts.map((item, index) => ({
                  id: Date.now().toString() + index,
                  name: item.name,
                  price: item.price.toString(),
                  stock: item.stock.toString(),
                  category: item.category,
                }))
              );
              navigation.goBack();
            },
          },
        ]
      );
    } catch (error: any) {
      Alert.alert('Error', 'Gagal menyimpan: ' + error.message);
    } finally {
      setLoading(false);
    }
  };

  return (
    <ScrollView style={styles.container} contentContainerStyle={styles.contentContainer}>
      <Text style={styles.title}>Form Produk</Text>
      <Text style={styles.subtitle}>Tambah Produk (Multi-Input)</Text>

      {products.map((item, index) => (
        <View key={item.id} style={styles.card}>
          <View style={styles.cardHeader}>
            <Text style={styles.cardTitle}>🍽️ Produk #{index + 1}</Text>
            {products.length > 1 && (
              <TouchableOpacity style={styles.removeButton} onPress={() => removeRow(item.id)}>
                <Text style={styles.removeButtonText}>Hapus</Text>
              </TouchableOpacity>
            )}
          </View>

          <Text style={styles.label}>Nama Produk *</Text>
          <TextInput
            style={styles.input}
            placeholder="Masukkan nama produk"
            value={item.name}
            onChangeText={(text) => updateField(item.id, 'name', text)}
          />

          <Text style={styles.label}>Harga (Rp) *</Text>
          <TextInput
            style={styles.input}
            placeholder="Masukkan harga"
            keyboardType="numeric"
            value={item.price}
            onChangeText={(text) => updateField(item.id, 'price', text)}
          />

          <Text style={styles.label}>Stok *</Text>
          <TextInput
            style={styles.input}
            placeholder="Masukkan jumlah stok"
            keyboardType="numeric"
            value={item.stock}
            onChangeText={(text) => updateField(item.id, 'stock', text)}
          />

          <Text style={styles.label}>Kategori *</Text>
          <TextInput
            style={styles.input}
            placeholder="Masukkan kategori"
            value={item.category}
            onChangeText={(text) => updateField(item.id, 'category', text)}
          />
        </View>
      ))}

      <TouchableOpacity style={styles.addButton} onPress={addRow}>
        <Text style={styles.addButtonText}>+ Tambah Produk Lain</Text>
      </TouchableOpacity>

      <View style={styles.actionRow}>
        <TouchableOpacity style={styles.cancelButton} onPress={() => navigation.goBack()}>
          <Text style={styles.cancelButtonText}>Batal</Text>
        </TouchableOpacity>
        <TouchableOpacity style={styles.saveButton} onPress={handleSubmit} disabled={loading}>
          {loading ? <ActivityIndicator color="#fff" /> : <Text style={styles.saveButtonText}>Simpan Semua</Text>}
        </TouchableOpacity>
      </View>
    </ScrollView>
  );
};

// ==================== STYLE ====================
const styles = StyleSheet.create({
  container: { flex: 1, backgroundColor: '#f5f6fa' },
  contentContainer: { padding: 20, paddingBottom: 40 },
  title: { fontSize: 24, fontWeight: 'bold', color: '#2d3436', marginBottom: 4 },
  subtitle: { fontSize: 14, color: '#636e72', marginBottom: 20 },
  card: {
    backgroundColor: '#fff',
    borderRadius: 12,
    padding: 16,
    marginBottom: 16,
    shadowColor: '#000',
    shadowOffset: { width: 0, height: 2 },
    shadowOpacity: 0.1,
    shadowRadius: 4,
    elevation: 3,
  },
  cardHeader: {
    flexDirection: 'row',
    justifyContent: 'space-between',
    alignItems: 'center',
    marginBottom: 12,
    borderBottomWidth: 1,
    borderBottomColor: '#eee',
    paddingBottom: 8,
  },
  cardTitle: { fontSize: 16, fontWeight: '600', color: '#0984e3' },
  removeButton: { backgroundColor: '#ff7675', paddingHorizontal: 12, paddingVertical: 4, borderRadius: 8 },
  removeButtonText: { color: '#fff', fontWeight: 'bold', fontSize: 12 },
  label: { fontSize: 14, fontWeight: '500', color: '#2d3436', marginBottom: 4, marginTop: 8 },
  input: {
    borderWidth: 1,
    borderColor: '#dfe6e9',
    borderRadius: 8,
    paddingHorizontal: 12,
    paddingVertical: 10,
    fontSize: 14,
    backgroundColor: '#f8f9fa',
  },
  addButton: { backgroundColor: '#00b894', paddingVertical: 14, borderRadius: 10, alignItems: 'center', marginVertical: 10 },
  addButtonText: { color: '#fff', fontWeight: 'bold', fontSize: 16 },
  actionRow: { flexDirection: 'row', justifyContent: 'space-between', marginTop: 20, gap: 12 },
  cancelButton: { flex: 1, backgroundColor: '#dfe6e9', paddingVertical: 14, borderRadius: 10, alignItems: 'center' },
  cancelButtonText: { color: '#2d3436', fontWeight: 'bold', fontSize: 16 },
  saveButton: { flex: 1, backgroundColor: '#0984e3', paddingVertical: 14, borderRadius: 10, alignItems: 'center' },
  saveButtonText: { color: '#fff', fontWeight: 'bold', fontSize: 16 },
});

export default TambahProdukScreen;