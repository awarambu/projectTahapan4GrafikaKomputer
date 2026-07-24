import React, { useState, useEffect } from 'react';
import {
  View,
  Text,
  FlatList,
  StyleSheet,
  RefreshControl,
} from 'react-native';
import { colors } from '../../theme/colors';
import { formatCurrency, formatDate } from '../../utils/helpers';
import Icon from 'react-native-vector-icons/Ionicons';

interface RiwayatTransaksiScreenProps {
  navigation: any;
}

const RiwayatTransaksiScreen: React.FC<RiwayatTransaksiScreenProps> = () => {
  const [transactions, setTransactions] = useState<any[]>([]);
  const [loading, setLoading] = useState(false);

  useEffect(() => {
    loadTransactions();
  }, []);

  const loadTransactions = async () => {
    setLoading(true);
    // Sementara pakai data dummy
    const dummyData = [
      {
        id: '1',
        receiptNumber: 'INV-2026-000001',
        total: 125000,
        items: 3,
        createdAt: new Date(),
        cashierName: 'Geby',
      },
      {
        id: '2',
        receiptNumber: 'INV-2026-000002',
        total: 75000,
        items: 2,
        createdAt: new Date(Date.now() - 3600000),
        cashierName: 'Geby',
      },
    ];
    setTimeout(() => {
      setTransactions(dummyData);
      setLoading(false);
    }, 500);
  };

  const renderItem = ({ item }: any) => (
    <View style={styles.card}>
      <View style={styles.cardHeader}>
        <Text style={styles.receiptNumber}>{item.receiptNumber}</Text>
        <Text style={styles.totalAmount}>{formatCurrency(item.total)}</Text>
      </View>
      <View style={styles.cardBody}>
        <Text style={styles.itemCount}>{item.items} item</Text>
        <Text style={styles.dateText}>{formatDate(item.createdAt)}</Text>
      </View>
      <Text style={styles.cashierText}>Kasir: {item.cashierName}</Text>
    </View>
  );

  return (
    <View style={styles.container}>
      {transactions.length === 0 && !loading ? (
        <View style={styles.emptyContainer}>
          <Icon name="receipt-outline" size={64} color={colors.border} />
          <Text style={styles.emptyText}>Belum ada transaksi</Text>
        </View>
      ) : (
        <FlatList
          data={transactions}
          renderItem={renderItem}
          keyExtractor={(item) => item.id}
          refreshControl={
            <RefreshControl refreshing={loading} onRefresh={loadTransactions} />
          }
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
  listContent: {
    padding: 16,
  },
  card: {
    backgroundColor: colors.surface,
    borderRadius: 12,
    padding: 16,
    marginBottom: 12,
    shadowColor: colors.shadow,
    shadowOffset: { width: 0, height: 2 },
    shadowOpacity: 0.1,
    shadowRadius: 4,
    elevation: 2,
  },
  cardHeader: {
    flexDirection: 'row',
    justifyContent: 'space-between',
    alignItems: 'center',
  },
  receiptNumber: {
    fontSize: 14,
    fontWeight: '600',
    color: colors.textPrimary,
  },
  totalAmount: {
    fontSize: 16,
    fontWeight: 'bold',
    color: colors.primary,
  },
  cardBody: {
    flexDirection: 'row',
    justifyContent: 'space-between',
    marginTop: 8,
  },
  itemCount: {
    fontSize: 14,
    color: colors.textTertiary,
  },
  dateText: {
    fontSize: 14,
    color: colors.textTertiary,
  },
  cashierText: {
    fontSize: 12,
    color: colors.textTertiary,
    marginTop: 8,
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

export default RiwayatTransaksiScreen;