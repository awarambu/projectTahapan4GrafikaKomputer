import React from 'react';
import {
  View,
  Text,
  TouchableOpacity,
  StyleSheet,
  ScrollView,
} from 'react-native';
import { colors } from '../../theme/colors';
import { formatCurrency, formatDate } from '../../utils/helpers';
import Icon from 'react-native-vector-icons/Ionicons';

interface StrukScreenProps {
  route: any;
  navigation: any;
}

const StrukScreen: React.FC<StrukScreenProps> = ({ route, navigation }) => {
  // Data dummy
  const transaction = {
    receiptNumber: 'INV-2026-000001',
    items: [
      { name: 'Product A', price: 25000, quantity: 2, subtotal: 50000 },
      { name: 'Product B', price: 15000, quantity: 1, subtotal: 15000 },
      { name: 'Product C', price: 60000, quantity: 1, subtotal: 60000 },
    ],
    subtotal: 125000,
    tax: 13750,
    total: 138750,
    paymentMethod: 'Tunai',
    paymentAmount: 140000,
    change: 1250,
    cashierName: 'Geby',
    createdAt: new Date(),
  };

  return (
    <ScrollView style={styles.container}>
      <View style={styles.receipt}>
        <View style={styles.headerReceipt}>
          <Text style={styles.storeName}>Toko Kasir Geby</Text>
          <Text style={styles.storeAddress}>Jl. Contoh No. 123, Kota</Text>
          <Text style={styles.storePhone}>Telp: 0812-3456-7890</Text>
          <View style={styles.divider} />
          <Text style={styles.receiptTitle}>STRUK PEMBELIAN</Text>
          <Text style={styles.receiptNumber}>{transaction.receiptNumber}</Text>
          <Text style={styles.receiptDate}>{formatDate(transaction.createdAt)}</Text>
          <View style={styles.divider} />
        </View>

        <View style={styles.itemsSection}>
          <View style={styles.itemHeader}>
            <Text style={[styles.itemHeaderText, { flex: 2 }]}>Item</Text>
            <Text style={[styles.itemHeaderText, { flex: 1, textAlign: 'center' }]}>Qty</Text>
            <Text style={[styles.itemHeaderText, { flex: 2, textAlign: 'right' }]}>Subtotal</Text>
          </View>
          {transaction.items.map((item, index) => (
            <View key={index} style={styles.itemRow}>
              <Text style={[styles.itemText, { flex: 2 }]}>{item.name}</Text>
              <Text style={[styles.itemText, { flex: 1, textAlign: 'center' }]}>{item.quantity}</Text>
              <Text style={[styles.itemText, { flex: 2, textAlign: 'right' }]}>
                {formatCurrency(item.subtotal)}
              </Text>
            </View>
          ))}
        </View>

        <View style={styles.divider} />

        <View style={styles.totalsSection}>
          <View style={styles.totalRow}>
            <Text style={styles.totalLabel}>Subtotal</Text>
            <Text style={styles.totalValue}>{formatCurrency(transaction.subtotal)}</Text>
          </View>
          <View style={styles.totalRow}>
            <Text style={styles.totalLabel}>PPN 11%</Text>
            <Text style={styles.totalValue}>{formatCurrency(transaction.tax)}</Text>
          </View>
          <View style={[styles.totalRow, styles.grandTotalRow]}>
            <Text style={styles.grandTotalLabel}>Total</Text>
            <Text style={styles.grandTotalValue}>{formatCurrency(transaction.total)}</Text>
          </View>
        </View>

        <View style={styles.divider} />

        <View style={styles.paymentSection}>
          <Text style={styles.paymentText}>Metode: {transaction.paymentMethod}</Text>
          <Text style={styles.paymentText}>Bayar: {formatCurrency(transaction.paymentAmount)}</Text>
          <Text style={styles.paymentText}>Kembali: {formatCurrency(transaction.change)}</Text>
        </View>

        <View style={styles.divider} />

        <View style={styles.footerReceipt}>
          <Text style={styles.cashierText}>Kasir: {transaction.cashierName}</Text>
          <Text style={styles.thankYou}>Terima kasih!</Text>
          <Text style={styles.footerText}>Barang yang sudah dibeli tidak dapat dikembalikan</Text>
        </View>
      </View>

      <View style={styles.actionButtons}>
        <TouchableOpacity
          style={[styles.button, styles.backButton]}
          onPress={() => navigation.navigate('Dashboard')}
        >
          <Icon name="home-outline" size={20} color={colors.textPrimary} />
          <Text style={styles.backButtonText}>Beranda</Text>
        </TouchableOpacity>
        <TouchableOpacity
          style={[styles.button, styles.printButton]}
          onPress={() => Alert.alert('Info', 'Fitur cetak struk')}
        >
          <Icon name="print-outline" size={20} color="#fff" />
          <Text style={styles.printButtonText}>Cetak</Text>
        </TouchableOpacity>
      </View>
    </ScrollView>
  );
};

// Import Alert
import { Alert } from 'react-native';

const styles = StyleSheet.create({
  container: {
    flex: 1,
    backgroundColor: colors.background,
  },
  receipt: {
    backgroundColor: colors.surface,
    margin: 16,
    padding: 20,
    borderRadius: 12,
    shadowColor: colors.shadow,
    shadowOffset: { width: 0, height: 2 },
    shadowOpacity: 0.1,
    shadowRadius: 4,
    elevation: 2,
  },
  headerReceipt: {
    alignItems: 'center',
  },
  storeName: {
    fontSize: 18,
    fontWeight: 'bold',
    color: colors.textPrimary,
  },
  storeAddress: {
    fontSize: 12,
    color: colors.textTertiary,
  },
  storePhone: {
    fontSize: 12,
    color: colors.textTertiary,
    marginBottom: 4,
  },
  receiptTitle: {
    fontSize: 14,
    fontWeight: '600',
    color: colors.textPrimary,
    marginTop: 4,
  },
  receiptNumber: {
    fontSize: 12,
    color: colors.textTertiary,
  },
  receiptDate: {
    fontSize: 12,
    color: colors.textTertiary,
  },
  divider: {
    height: 1,
    backgroundColor: colors.border,
    width: '100%',
    marginVertical: 8,
  },
  itemsSection: {
    marginVertical: 4,
  },
  itemHeader: {
    flexDirection: 'row',
    borderBottomWidth: 1,
    borderBottomColor: colors.border,
    paddingBottom: 4,
    marginBottom: 4,
  },
  itemHeaderText: {
    fontSize: 12,
    fontWeight: '600',
    color: colors.textTertiary,
  },
  itemRow: {
    flexDirection: 'row',
    paddingVertical: 3,
  },
  itemText: {
    fontSize: 14,
    color: colors.textSecondary,
  },
  totalsSection: {
    marginVertical: 4,
  },
  totalRow: {
    flexDirection: 'row',
    justifyContent: 'space-between',
    paddingVertical: 2,
  },
  totalLabel: {
    fontSize: 14,
    color: colors.textTertiary,
  },
  totalValue: {
    fontSize: 14,
    color: colors.textSecondary,
  },
  grandTotalRow: {
    borderTopWidth: 1,
    borderTopColor: colors.border,
    paddingTop: 8,
    marginTop: 4,
  },
  grandTotalLabel: {
    fontSize: 18,
    fontWeight: 'bold',
    color: colors.textPrimary,
  },
  grandTotalValue: {
    fontSize: 18,
    fontWeight: 'bold',
    color: colors.primary,
  },
  paymentSection: {
    marginVertical: 4,
  },
  paymentText: {
    fontSize: 14,
    color: colors.textSecondary,
    paddingVertical: 2,
  },
  footerReceipt: {
    alignItems: 'center',
  },
  cashierText: {
    fontSize: 12,
    color: colors.textTertiary,
  },
  thankYou: {
    fontSize: 16,
    fontWeight: 'bold',
    color: colors.textPrimary,
    marginTop: 4,
  },
  footerText: {
    fontSize: 10,
    color: colors.textTertiary,
    marginTop: 4,
    textAlign: 'center',
  },
  actionButtons: {
    flexDirection: 'row',
    paddingHorizontal: 16,
    paddingBottom: 24,
    gap: 12,
  },
  button: {
    flex: 1,
    flexDirection: 'row',
    paddingVertical: 12,
    borderRadius: 8,
    alignItems: 'center',
    justifyContent: 'center',
    gap: 8,
  },
  backButton: {
    backgroundColor: colors.background,
    borderWidth: 1,
    borderColor: colors.border,
  },
  printButton: {
    backgroundColor: colors.primary,
  },
  backButtonText: {
    fontSize: 14,
    fontWeight: '600',
    color: colors.textPrimary,
  },
  printButtonText: {
    fontSize: 14,
    fontWeight: '600',
    color: '#fff',
  },
});

export default StrukScreen;