import React, { useState } from 'react';
import {
  View,
  Text,
  FlatList,
  TouchableOpacity,
  StyleSheet,
  TextInput,
  ActivityIndicator,
} from 'react-native';
import { useProduct } from '../../context/ProdukContext';
import { useCart } from '../../context/CartContext';
import { colors } from '../../theme/colors';
import { formatCurrency } from '../../utils/helpers';
import Icon from 'react-native-vector-icons/Ionicons';

interface PilihProdukScreenProps {
  navigation: any;
}

const PilihProdukScreen: React.FC<PilihProdukScreenProps> = ({ navigation }) => {
  const { products, loading, searchProducts } = useProduct();
  const { addToCart, getTotalItems, getSubtotal } = useCart();
  const [searchText, setSearchText] = useState('');
  const [filteredProducts, setFilteredProducts] = useState<any[]>([]);
  const [isSearching, setIsSearching] = useState(false);

  const handleSearch = async (text: string) => {
    setSearchText(text);
    if (text.trim().length >= 2) {
      setIsSearching(true);
      const results = await searchProducts(text);
      setFilteredProducts(results);
      setIsSearching(false);
    } else {
      setFilteredProducts([]);
    }
  };

  const handleAddToCart = (product: any) => {
    addToCart(product);
    Alert.alert('Sukses', `${product.name} ditambahkan`);
  };

  const displayProducts = searchText.trim().length >= 2 ? filteredProducts : products;

  const renderItem = ({ item }: any) => (
    <TouchableOpacity
      style={styles.productItem}
      onPress={() => handleAddToCart(item)}
    >
      <View style={styles.productInfo}>
        <Text style={styles.productName}>{item.name}</Text>
        <Text style={styles.productCategory}>{item.category}</Text>
        <Text style={styles.productPrice}>{formatCurrency(item.price)}</Text>
      </View>
      <View style={styles.productRight}>
        <View style={[styles.stockBadge, item.stock < 5 && styles.stockLow]}>
          <Text style={styles.stockText}>Stok: {item.stock}</Text>
        </View>
        <TouchableOpacity
          style={styles.addButton}
          onPress={() => handleAddToCart(item)}
        >
          <Icon name="add" size={20} color="#fff" />
        </TouchableOpacity>
      </View>
    </TouchableOpacity>
  );

  return (
    <View style={styles.container}>
      <View style={styles.header}>
        <Text style={styles.title}>Pilih Produk</Text>
        <TouchableOpacity
          style={styles.cartButton}
          onPress={() => navigation.navigate('Keranjang')}
        >
          <Icon name="cart-outline" size={24} color={colors.primary} />
          {getTotalItems() > 0 && (
            <View style={styles.cartBadge}>
              <Text style={styles.cartBadgeText}>{getTotalItems()}</Text>
            </View>
          )}
        </TouchableOpacity>
      </View>

      <View style={styles.searchContainer}>
        <Icon name="search-outline" size={20} color={colors.textTertiary} style={styles.searchIcon} />
        <TextInput
          style={styles.searchInput}
          placeholder="Cari produk..."
          value={searchText}
          onChangeText={handleSearch}
          placeholderTextColor={colors.textTertiary}
        />
        {isSearching && <ActivityIndicator size="small" color={colors.primary} />}
      </View>

      {displayProducts.length === 0 && !loading ? (
        <View style={styles.emptyContainer}>
          <Icon name="cube-outline" size={64} color={colors.border} />
          <Text style={styles.emptyText}>
            {searchText.trim().length >= 2 ? 'Produk tidak ditemukan' : 'Tidak ada produk'}
          </Text>
        </View>
      ) : (
        <FlatList
          data={displayProducts}
          renderItem={renderItem}
          keyExtractor={(item) => item.id || Math.random().toString()}
          contentContainerStyle={styles.listContent}
        />
      )}
    </View>
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
  title: {
    fontSize: 20,
    fontWeight: 'bold',
    color: colors.textPrimary,
  },
  cartButton: {
    position: 'relative',
    padding: 4,
  },
  cartBadge: {
    position: 'absolute',
    top: -4,
    right: -4,
    backgroundColor: colors.danger,
    borderRadius: 10,
    minWidth: 20,
    height: 20,
    alignItems: 'center',
    justifyContent: 'center',
    paddingHorizontal: 4,
  },
  cartBadgeText: {
    color: '#fff',
    fontSize: 11,
    fontWeight: 'bold',
  },
  searchContainer: {
    flexDirection: 'row',
    alignItems: 'center',
    backgroundColor: colors.surface,
    margin: 12,
    paddingHorizontal: 12,
    borderRadius: 10,
    borderWidth: 1,
    borderColor: colors.border,
  },
  searchIcon: {
    marginRight: 8,
  },
  searchInput: {
    flex: 1,
    paddingVertical: 10,
    fontSize: 16,
    color: colors.textPrimary,
  },
  listContent: {
    padding: 12,
  },
  productItem: {
    backgroundColor: colors.surface,
    borderRadius: 12,
    padding: 16,
    marginBottom: 12,
    flexDirection: 'row',
    justifyContent: 'space-between',
    alignItems: 'center',
    shadowColor: colors.shadow,
    shadowOffset: { width: 0, height: 2 },
    shadowOpacity: 0.1,
    shadowRadius: 4,
    elevation: 2,
  },
  productInfo: {
    flex: 1,
  },
  productName: {
    fontSize: 16,
    fontWeight: '600',
    color: colors.textPrimary,
  },
  productCategory: {
    fontSize: 14,
    color: colors.textTertiary,
    marginTop: 2,
  },
  productPrice: {
    fontSize: 16,
    fontWeight: 'bold',
    color: colors.primary,
    marginTop: 4,
  },
  productRight: {
    alignItems: 'flex-end',
    gap: 8,
  },
  stockBadge: {
    backgroundColor: colors.success,
    paddingHorizontal: 8,
    paddingVertical: 2,
    borderRadius: 4,
  },
  stockLow: {
    backgroundColor: colors.danger,
  },
  stockText: {
    color: '#fff',
    fontSize: 11,
    fontWeight: '600',
  },
  addButton: {
    backgroundColor: colors.primary,
    width: 36,
    height: 36,
    borderRadius: 18,
    alignItems: 'center',
    justifyContent: 'center',
  },
  emptyContainer: {
    flex: 1,
    justifyContent: 'center',
    alignItems: 'center',
    padding: 20,
  },
  emptyText: {
    fontSize: 18,
    fontWeight: 'bold',
    color: colors.textTertiary,
    marginTop: 16,
  },
});

// Import Alert
import { Alert } from 'react-native';

export default PilihProdukScreen;