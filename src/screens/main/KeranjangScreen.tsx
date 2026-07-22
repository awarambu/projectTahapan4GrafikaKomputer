import React from 'react';
import {
  View,
  Text,
  FlatList,
  TouchableOpacity,
  StyleSheet,
  Alert,
} from 'react-native';
import { useCart } from '../../context/CartContext';
import { useProduct } from '../../context/ProdukContext';
import { colors } from '../../theme/colors';
import { formatCurrency } from '../../utils/helpers';
import Icon from 'react-native-vector-icons/Ionicons';

interface KeranjangScreenProps {
  navigation: any;
}

const KeranjangScreen: React.FC<KeranjangScreenProps> = ({ navigation }) => {
  const { cart, updateQuantity, removeFromCart, clearCart, getTotalItems, getSubtotal } = useCart();

  const subtotal = getSubtotal();
  const tax = Math.round(subtotal * 0.11); // PPN 11%
  const total = subtotal + tax;

  const handleCheckout = () => {
    if (cart.length === 0) {
      Alert.alert('Info', 'Keranjang kosong');
      return;
    }
    navigation.navigate('PilihProduk');
  };

  const renderItem = ({ item }: any) => (
    <View style={styles.cartItem}>
      <View style={styles.itemInfo}>
        <Text style={styles.itemName}>{item.product.name}</Text>
        <Text style={styles.itemPrice}>{formatCurrency(item.product.price)}</Text>
      </View>
      <View style={styles.itemActions}>
        <TouchableOpacity
          style={styles.quantityButton}
          onPress={() => updateQuantity(item.product.id, item.quantity - 1)}
        >
          <Icon name="remove" size={16} color="#fff" />
        </TouchableOpacity>
        <Text style={styles.quantityText}>{item.quantity}</Text>
        <TouchableOpacity
          style={styles.quantityButton}
          onPress={() => updateQuantity(item.product.id, item.quantity + 1)}
        >
          <Icon name="add" size={16} color="#fff" />
        </TouchableOpacity>
        <TouchableOpacity
          style={styles.removeButton}
          onPress={() => removeFromCart(item.product.id)}
        >
          <Icon name="trash-outline" size={18} color={colors.danger} />
        </TouchableOpacity>
      </View>
    </View>
  );

  return (
    <View style={styles.container}>
      <View style={styles.header}>
        <Text style={styles.title}>Keranjang</Text>
        {cart.length > 0 && (
          <TouchableOpacity onPress={clearCart}>
            <Text style={styles.clearText}>Kosongkan</Text>
          </TouchableOpacity>
        )}
      </View>

      {cart.length === 0 ? (
        <View style={styles.emptyContainer}>
          <Icon name="cart-outline" size={64} color={colors.border} />
          <Text style={styles.emptyText}>Keranjang kosong</Text>
          <Text style={styles.emptySubText}>Tambahkan produk dari daftar produk</Text>
        </View>
      ) : (
        <>
          <FlatList
            data={cart}
            renderItem={renderItem}
            keyExtractor={(item) => item.product.id || Math.random().toString()}
            contentContainerStyle={styles.listContent}
          />

          <View style={styles.footer}>
            <View style={styles.summaryRow}>
              <Text style={styles.summaryLabel}>Subtotal</Text>
              <Text style={styles.summaryValue}>{formatCurrency(subtotal)}</Text>
            </View>
            <View style={styles.summaryRow}>
              <Text style={styles.summaryLabel}>PPN 11%</Text>
              <Text style={styles.summaryValue}>{formatCurrency(tax)}</Text>
            </View>
            <View style={[styles.summaryRow, styles.totalRow]}>
              <Text style={styles.totalLabel}>Total</Text>
              <Text style={styles.totalValue}>{formatCurrency(total)}</Text>
            </View>

            <TouchableOpacity style={styles.checkoutButton} onPress={handleCheckout}>
              <Text style={styles.checkoutButtonText}>Lanjutkan Transaksi</Text>
              <Icon name="arrow-forward" size={20} color="#fff" />
            </TouchableOpacity>
          </View>
        </>
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
  clearText: {
    color: colors.danger,
    fontSize: 14,
    fontWeight: '600',
  },
  listContent: {
    padding: 16,
  },
  cartItem: {
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
  itemInfo: {
    flex: 1,
  },
  itemName: {
    fontSize: 16,
    fontWeight: '600',
    color: colors.textPrimary,
  },
  itemPrice: {
    fontSize: 14,
    color: colors.primary,
    marginTop: 2,
  },
  itemActions: {
    flexDirection: 'row',
    alignItems: 'center',
    gap: 8,
  },
  quantityButton: {
    backgroundColor: colors.primary,
    width: 28,
    height: 28,
    borderRadius: 14,
    alignItems: 'center',
    justifyContent: 'center',
  },
  quantityText: {
    fontSize: 16,
    fontWeight: '600',
    color: colors.textPrimary,
    width: 30,
    textAlign: 'center',
  },
  removeButton: {
    padding: 4,
  },
  footer: {
    backgroundColor: colors.surface,
    padding: 16,
    borderTopWidth: 1,
    borderTopColor: colors.border,
  },
  summaryRow: {
    flexDirection: 'row',
    justifyContent: 'space-between',
    paddingVertical: 4,
  },
  summaryLabel: {
    fontSize: 14,
    color: colors.textTertiary,
  },
  summaryValue: {
    fontSize: 14,
    color: colors.textSecondary,
  },
  totalRow: {
    borderTopWidth: 1,
    borderTopColor: colors.border,
    paddingTop: 12,
    marginTop: 4,
  },
  totalLabel: {
    fontSize: 18,
    fontWeight: 'bold',
    color: colors.textPrimary,
  },
  totalValue: {
    fontSize: 18,
    fontWeight: 'bold',
    color: colors.primary,
  },
  checkoutButton: {
    flexDirection: 'row',
    backgroundColor: colors.primary,
    borderRadius: 12,
    paddingVertical: 14,
    alignItems: 'center',
    justifyContent: 'center',
    marginTop: 12,
    gap: 8,
  },
  checkoutButtonText: {
    color: '#fff',
    fontSize: 16,
    fontWeight: '600',
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
  emptySubText: {
    fontSize: 14,
    color: colors.textTertiary,
    marginTop: 8,
  },
});

export default KeranjangScreen;