import React, { useState, useEffect } from 'react';
import {
  View,
  Text,
  StyleSheet,
  TouchableOpacity,
  ScrollView,
  RefreshControl,
} from 'react-native';
import { useProduct } from '../../context/ProdukContext';
import { useCart } from '../../context/CartContext';
import { colors } from '../../theme/colors';
import { formatCurrency } from '../../utils/helpers';
import Icon from 'react-native-vector-icons/Ionicons';

interface DashboardScreenProps {
  navigation: any;
}

const DashboardScreen: React.FC<DashboardScreenProps> = ({ navigation }) => {
  const { products, loading, fetchProducts } = useProduct();
  const { getTotalItems, getSubtotal } = useCart();
  const [refreshing, setRefreshing] = useState(false);

  const totalProducts = products.length;
  const totalItems = getTotalItems();
  const subtotal = getSubtotal();

  const onRefresh = async () => {
    setRefreshing(true);
    await fetchProducts();
    setRefreshing(false);
  };

  const menuItems = [
    { id: '1', title: 'Tambah Produk', icon: 'add-circle', color: colors.primary, screen: 'FormProduk', params: { mode: 'add' } },
    { id: '2', title: 'Daftar Produk', icon: 'grid', color: colors.info, screen: 'Produk' },
    { id: '3', title: 'Transaksi', icon: 'cart', color: colors.success, screen: 'PilihProduk' },
    { id: '4', title: 'Riwayat', icon: 'time', color: colors.warning, screen: 'RiwayatTransaksi' },
  ];

  return (
    <ScrollView
      style={styles.container}
      refreshControl={<RefreshControl refreshing={refreshing} onRefresh={onRefresh} />}
    >
      <View style={styles.header}>
        <Text style={styles.greeting}>Selamat Datang!</Text>
        <Text style={styles.subGreeting}>Aplikasi Kasir Digital</Text>
      </View>

      <View style={styles.statsContainer}>
        <View style={styles.statCard}>
          <Icon name="cube-outline" size={24} color={colors.primary} />
          <Text style={styles.statNumber}>{totalProducts}</Text>
          <Text style={styles.statLabel}>Total Produk</Text>
        </View>
        <View style={styles.statCard}>
          <Icon name="cart-outline" size={24} color={colors.success} />
          <Text style={styles.statNumber}>{totalItems}</Text>
          <Text style={styles.statLabel}>Keranjang</Text>
        </View>
        <View style={styles.statCard}>
          <Icon name="cash-outline" size={24} color={colors.warning} />
          <Text style={[styles.statNumber, { fontSize: 14 }]}>{formatCurrency(subtotal)}</Text>
          <Text style={styles.statLabel}>Total Belanja</Text>
        </View>
      </View>

      <Text style={styles.sectionTitle}>Menu Cepat</Text>
      <View style={styles.menuGrid}>
        {menuItems.map((item) => (
          <TouchableOpacity
            key={item.id}
            style={styles.menuItem}
            onPress={() => navigation.navigate(item.screen, item.params || {})}
          >
            <View style={[styles.menuIcon, { backgroundColor: item.color + '20' }]}>
              <Icon name={item.icon} size={28} color={item.color} />
            </View>
            <Text style={styles.menuTitle}>{item.title}</Text>
          </TouchableOpacity>
        ))}
      </View>

      <View style={styles.footer}>
        <Text style={styles.footerText}>Aplikasi Kasir v1.0.0</Text>
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
    backgroundColor: colors.surface,
    padding: 20,
    paddingTop: 48,
    borderBottomWidth: 1,
    borderBottomColor: colors.border,
  },
  greeting: {
    fontSize: 24,
    fontWeight: 'bold',
    color: colors.textPrimary,
  },
  subGreeting: {
    fontSize: 14,
    color: colors.textTertiary,
    marginTop: 4,
  },
  statsContainer: {
    flexDirection: 'row',
    padding: 16,
    gap: 12,
  },
  statCard: {
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
  statNumber: {
    fontSize: 20,
    fontWeight: 'bold',
    color: colors.textPrimary,
    marginTop: 8,
  },
  statLabel: {
    fontSize: 12,
    color: colors.textTertiary,
    marginTop: 2,
  },
  sectionTitle: {
    fontSize: 18,
    fontWeight: '600',
    color: colors.textPrimary,
    paddingHorizontal: 16,
    marginTop: 8,
    marginBottom: 12,
  },
  menuGrid: {
    flexDirection: 'row',
    flexWrap: 'wrap',
    paddingHorizontal: 12,
    gap: 12,
  },
  menuItem: {
    width: '22%',
    aspectRatio: 1,
    backgroundColor: colors.surface,
    borderRadius: 12,
    padding: 12,
    alignItems: 'center',
    justifyContent: 'center',
    shadowColor: colors.shadow,
    shadowOffset: { width: 0, height: 2 },
    shadowOpacity: 0.1,
    shadowRadius: 4,
    elevation: 2,
  },
  menuIcon: {
    width: 48,
    height: 48,
    borderRadius: 24,
    alignItems: 'center',
    justifyContent: 'center',
    marginBottom: 8,
  },
  menuTitle: {
    fontSize: 11,
    textAlign: 'center',
    color: colors.textSecondary,
    fontWeight: '500',
  },
  footer: {
    padding: 24,
    alignItems: 'center',
  },
  footerText: {
    fontSize: 12,
    color: colors.textTertiary,
  },
});

export default DashboardScreen;