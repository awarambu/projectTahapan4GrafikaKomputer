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
import { addProduct, validateProduct } from '../../services/productService';

type RootStackParamList = {
  FormProduk: undefined;
  PilihProduk: undefined;
};
type NavigationProp = StackNavigationProp<RootStackParamList, 'FormProduk'>;

interface Props {
  navigation: NavigationProp;
}

const FormProdukScreen: React.FC<Props> = ({ navigation }) => {
  const [name, setName] = useState('');
  const [price, setPrice] = useState('');
  const [stock, setStock] = useState('');
  const [category, setCategory] = useState('');
  const [loading, setLoading] = useState(false);

  const handleSubmit = async () => {
    // Validasi sederhana
    if (!name.trim() || !price.trim() || !stock.trim() || !category.trim()) {
      Alert.alert('Error', 'Semua field harus diisi!');
      return;
    }

    const productData = {
      name: name.trim(),
      price: parseInt(price, 10),
      stock: parseInt(stock, 10),
      category: category.trim(),
    };

    // Validasi dengan fungsi dari service
    const validation = validateProduct(productData);
    if (Object.keys(validation).length > 0) {
      Alert.alert('Error', JSON.stringify(validation));
      return;
    }

    setLoading(true);
    try {
      await addProduct(productData);
      Alert.alert('Sukses', 'Produk berhasil ditambahkan!', [
        {
          text: 'OK',
          onPress: () => {
            // Reset form
            setName('');
            setPrice('');
            setStock('');
            setCategory('');
            // Kembali ke daftar produk (opsional)
            navigation.navigate('PilihProduk');
          },
        },
      ]);
    } catch (error: any) {
      Alert.alert('Error', 'Gagal menambahkan produk: ' + error.message);
    } finally {
      setLoading(false);
    }
  };

  return (
    <ScrollView style={styles.container} contentContainerStyle={styles.contentContainer}>
      <Text style={styles.title}>Form Produk</Text>
      <Text style={styles.subtitle}>Tambah Produk</Text>

      <View style={styles.card}>
        <Text style={styles.label}>Nama Produk *</Text>
        <TextInput
          style={styles.input}
          placeholder="Masukkan nama produk"
          value={name}
          onChangeText={setName}
        />

        <Text style={styles.label}>Harga (Rp) *</Text>
        <TextInput
          style={styles.input}
          placeholder="Masukkan harga"
          keyboardType="numeric"
          value={price}
          onChangeText={setPrice}
        />

        <Text style={styles.label}>Stok *</Text>
        <TextInput
          style={styles.input}
          placeholder="Masukkan jumlah stok"
          keyboardType="numeric"
          value={stock}
          onChangeText={setStock}
        />

        <Text style={styles.label}>Kategori *</Text>
        <TextInput
          style={styles.input}
          placeholder="Masukkan kategori"
          value={category}
          onChangeText={setCategory}
        />
      </View>

      <View style={styles.actionRow}>
        <TouchableOpacity style={styles.cancelButton} onPress={() => navigation.goBack()}>
          <Text style={styles.cancelButtonText}>Batal</Text>
        </TouchableOpacity>
        <TouchableOpacity style={styles.saveButton} onPress={handleSubmit} disabled={loading}>
          {loading ? <ActivityIndicator color="#fff" /> : <Text style={styles.saveButtonText}>Simpan</Text>}
        </TouchableOpacity>
      </View>
    </ScrollView>
  );
};

const styles = StyleSheet.create({
  container: {
    flex: 1,
    backgroundColor: '#f5f6fa',
  },
  contentContainer: {
    padding: 20,
    paddingBottom: 40,
  },
  title: {
    fontSize: 24,
    fontWeight: 'bold',
    color: '#2d3436',
    marginBottom: 4,
  },
  subtitle: {
    fontSize: 16,
    color: '#636e72',
    marginBottom: 20,
  },
  card: {
    backgroundColor: '#ffffff',
    borderRadius: 12,
    padding: 16,
    marginBottom: 16,
    shadowColor: '#000',
    shadowOffset: { width: 0, height: 2 },
    shadowOpacity: 0.1,
    shadowRadius: 4,
    elevation: 3,
  },
  label: {
    fontSize: 14,
    fontWeight: '500',
    color: '#2d3436',
    marginBottom: 4,
    marginTop: 8,
  },
  input: {
    borderWidth: 1,
    borderColor: '#dfe6e9',
    borderRadius: 8,
    paddingHorizontal: 12,
    paddingVertical: 10,
    fontSize: 14,
    backgroundColor: '#f8f9fa',
  },
  actionRow: {
    flexDirection: 'row',
    justifyContent: 'space-between',
    marginTop: 10,
    gap: 12,
  },
  cancelButton: {
    flex: 1,
    backgroundColor: '#dfe6e9',
    paddingVertical: 14,
    borderRadius: 10,
    alignItems: 'center',
  },
  cancelButtonText: {
    color: '#2d3436',
    fontWeight: 'bold',
    fontSize: 16,
  },
  saveButton: {
    flex: 1,
    backgroundColor: '#0984e3',
    paddingVertical: 14,
    borderRadius: 10,
    alignItems: 'center',
  },
  saveButtonText: {
    color: '#fff',
    fontWeight: 'bold',
    fontSize: 16,
  },
});

export default FormProdukScreen;