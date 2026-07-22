import React, { useState, useEffect } from 'react';
import {
  View,
  Text,
  StyleSheet,
  ScrollView,
  TouchableOpacity,
  Alert,
  ActivityIndicator,
} from 'react-native';
import { useProduct } from '../../context/ProdukContext';
import { useCart } from '../../context/CartContext';
import { colors } from '../../theme/colors';
import { formatCurrency, formatDate } from '../../utils/helpers';
import Icon from 'react-native-vector-icons/Ionicons';

interface DetailProdukScreenProps {
  route: any;
  navigation: any;
}

const DetailProdukScreen: React.FC<DetailProdukScreenProps> = ({ route, navigation }) => {
  const { productId } = route.params;
  const { getProduct, deleteProduct } = useProduct();
  const { addToCart } = useCart();
  const [product, setProduct] = useState<any>(null);
  const [loading, setLoading] = useState(true);

  useEffect(() => {
    loadProduct();
  }, []);

  const loadProduct = async () => {
    const data = await getProduct(productId);
    setProduct(data);
    setLoading(false);
  };

  const handleDelete = () => {
    Alert.alert(
      'Hapus Produk',
      `Yakin hapus "${product?.name}"?`,
      [
        { text: 'Batal', style: 'cancel' },
        {
          text: 'Hapus',
          style: 'destructive',
          onPress: async () => {
            if (product?.id) {
              const result = await deleteProduct(product.id);
              if (result.success) {
                Alert.alert('Sukses', 'Produk dihapus', [
                  { text: 'OK', onPress: () => navigation.goBack() },
                ]);
              } else {
                Alert.alert('Error', result.message || 'Gagal hapus');
              }
            }
          },
        },
      ]
    );
  };

  const handleAddToCart = () => {
    if (product) {
      addToCart(product);
      Alert.alert('Sukses', `${product.name} ditambahkan ke keranjang`);
    }
  };

  if (loading) {
    return (
      <View style={styles.centerContainer}>
        <ActivityIndicator size="large" color={colors.primary} />
        <Text style={styles.loadingText}>Memuat detail...</Text>
      </View>
    );
  }

  if (!product) {
    return (
      <View style={styles.centerContainer}>
        <Icon name="alert-circle-outline" size={64} color={colors.border} />
        <Text style={styles.errorText}>Produk tidak ditemukan</Text>
        <TouchableOpacity style={styles.backButton} onPress={() => navigation.goBack()}>
          <Text style={styles.backButtonText}>Kembali</Text>
        </TouchableOpacity>
      </View>
    );
  }

  return (
    <ScrollView style={styles.container}>
      <View style={styles.header}>
        <View style={styles.headerLeft}>
          <TouchableOpacity onPress={() => navigation.goBack()}>
            <Icon name="arrow-back" size={24} color={colors.textPrimary} />
          </TouchableOpacity>
          <Text style={styles.headerTitle}>Detail Produk</Text>
        </View>
        <View style={styles.headerRight}>
          <TouchableOpacity
            style={styles.headerButton}
            onPress={() => navigation.navigate('FormProduk', { mode: 'edit', productId: product.id })}
          >
            <Icon name="create-outline" size={22} color={colors.primary} />
          </TouchableOpacity>
          <TouchableOpacity style={styles.headerButton} onPress={handleDelete}>
            <Icon name="trash-outline" size={22} color={colors.danger} />
          </TouchableOpacity>
        </View>
      </View>

      <View style={styles.content}>
        <View style={styles.infoCard}>
          <Text style={styles.productName}>{product.name}</Text>
          <View style={styles.categoryBadge}>
            <Text style={styles.categoryText}>{product.category}</Text>
          </View>
        </View>

        <View style={styles.statsRow}>
          <View style={styles.statBox}>
            <Text style={styles.statLabel}>Harga</Text>
            <Text style={styles.statValue}>{formatCurrency(product.price)}</Text>
          </View>
          <View style={styles.statBox}>
            <Text style={styles.statLabel}>Stok</Text>
            <Text style={[styles.statValue, product.stock < 5 && { color: colors.danger }]}>
              {product.stock}
            </Text>
          </View>
        </View>

        <View style={styles.actionButtons}>
          <TouchableOpacity style={styles.addToCartButton} onPress={handleAddToCart}>
            <Icon name="cart-outline" size={20} color="#fff" />
            <Text style={styles.addToCartText}>Tambah ke Keranjang</Text>
          </TouchableOpacity>
        </View>

        <View style={styles.infoSection}>
          <Text style={styles.sectionTitle}>Informasi Tambahan</Text>
          <View style={styles.infoRow}>
            <Text style={styles.infoLabel}>ID Produk</Text>
            <Text style={styles.infoValue}>{product.id}</Text>
          </View>
          <View style={styles.infoRow}>
            <Text style={styles.infoLabel}>Dibuat</Text>
            <Text style={styles.infoValue}>{formatDate(product.createdAt)}</Text>
          </View>
          <View style={styles.infoRow}>
            <Text style={styles.infoLabel}>Terakhir Update</Text>
            <Text style={styles.infoValue}>{formatDate(product.updatedAt)}</Text>
          </View>
        </View>
      </View>
    </ScrollView>
  );
};

const styles = StyleSheet.create({
  container: {
    flex: 1,
    backgroundColor: colors.background,
  },
  header: {
    flexDirection: 'row',
    justifyContent: 'space-between',
    alignItems: 'center',
    padding: 16,
    backgroundColor: colors.surface,
    borderBottomWidth: 1,
    borderBottomColor: colors.border,
  },
  headerLeft: {
    flexDirection: 'row',
    alignItems: 'center',
    gap: 12,
  },
  headerTitle: {
    fontSize: 18,
    fontWeight: '600',
    color: colors.textPrimary,
  },
  headerRight: {
    flexDirection: 'row',
    gap: 16,
  },
  headerButton: {
    padding: 4,
  },
  content: {
    padding: 16,
  },
  infoCard: {
    backgroundColor: colors.surface,
    borderRadius: 12,
    padding: 16,
    marginBottom: 16,
    shadowColor: colors.shadow,
    shadowOffset: { width: 0, height: 2 },
    shadowOpacity: 0.1,
    shadowRadius: 4,
    elevation: 2,
  },
  productName: {
    fontSize: 22,
    fontWeight: 'bold',
    color: colors.textPrimary,
  },
  categoryBadge: {
    backgroundColor: colors.primary + '20',
    paddingHorizontal: 12,
    paddingVertical: 4,
    borderRadius: 16,
    alignSelf: 'flex-start',
    marginTop: 8,
  },
  categoryText: {
    color: colors.primary,
    fontSize: 14,
    fontWeight: '600',
  },
  statsRow: {
    flexDirection: 'row',
    gap: 12,
    marginBottom: 16,
  },
  statBox: {
    flex: 1,
    backgroundColor: colors.surface,
    borderRadius: 12,
    padding: 16,
    alignItems: 'center',
    shadowColor: colors.shadow,
    shadowOffset: { width: 0, height: 2 },
    shadowOpacity: 0.1,
    shadowRadius: 4,
    elevation: 2,
  },
  statLabel: {
    fontSize: 14,
    color: colors.textTertiary,
  },
  statValue: {
    fontSize: 20,
    fontWeight: 'bold',
    color: colors.textPrimary,
    marginTop: 4,
  },
  actionButtons: {
    marginBottom: 16,
  },
  addToCartButton: {
    flexDirection: 'row',
    backgroundColor: colors.primary,
    borderRadius: 12,
    paddingVertical: 14,
    alignItems: 'center',
    justifyContent: 'center',
    gap: 8,
  },
  addToCartText: {
    color: '#fff',
    fontSize: 16,
    fontWeight: '600',
  },
  infoSection: {
    backgroundColor: colors.surface,
    borderRadius: 12,
    padding: 16,
    shadowColor: colors.shadow,
    shadowOffset: { width: 0, height: 2 },
    shadowOpacity: 0.1,
    shadowRadius: 4,
    elevation: 2,
  },
  sectionTitle: {
    fontSize: 16,
    fontWeight: '600',
    color: colors.textPrimary,
    marginBottom: 12,
  },
  infoRow: {
    flexDirection: 'row',
    justifyContent: 'space-between',
    paddingVertical: 8,
    borderBottomWidth: 1,
    borderBottomColor: colors.border,
  },
  infoLabel: {
    fontSize: 14,
    color: colors.textTertiary,
  },
  infoValue: {
    fontSize: 14,
    color: colors.textPrimary,
    fontWeight: '500',
  },
  centerContainer: {
    flex: 1,
    justifyContent: 'center',
    alignItems: 'center',
    padding: 20,
    backgroundColor: colors.background,
  },
  loadingText: {
    marginTop: 12,
    fontSize: 16,
    color: colors.textTertiary,
  },
  errorText: {
    fontSize: 18,
    fontWeight: 'bold',
    color: colors.textTertiary,
    marginTop: 16,
  },
  backButton: {
    marginTop: 16,
    backgroundColor: colors.primary,
    paddingHorizontal: 24,
    paddingVertical: 12,
    borderRadius: 8,
  },
  backButtonText: {
    color: '#fff',
    fontWeight: 'bold',
    fontSize: 16,
  },
});

export default DetailProdukScreen;