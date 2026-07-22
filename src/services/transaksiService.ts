import {
  db,
  collection,
  addDoc,
  getDocs,
  query,
  orderBy,
  where,
  Timestamp,
  runTransaction,
  doc,
} from './firebase';
import { Transaction, CartItem } from '../types/transaction.types';
import { Product } from '../types/product.types';

const COLLECTION = 'transactions';

// CREATE Transaction
export const createTransaction = async (
  transaction: Omit<Transaction, 'id' | 'createdAt'>,
  items: CartItem[]
): Promise<string> => {
  try {
    // Gunakan runTransaction untuk update stok
    const docRef = await runTransaction(db, async (transaction) => {
      // Update stok untuk setiap item
      for (const item of items) {
        const productRef = doc(db, 'products', item.product.id!);
        const productSnap = await transaction.get(productRef);
        
        if (!productSnap.exists()) {
          throw new Error(`Produk ${item.product.name} tidak ditemukan`);
        }
        
        const currentStock = productSnap.data().stock || 0;
        if (currentStock < item.quantity) {
          throw new Error(`Stok ${item.product.name} tidak mencukupi`);
        }
        
        transaction.update(productRef, {
          stock: currentStock - item.quantity,
          updatedAt: Timestamp.now(),
        });
      }
      
      // Simpan transaksi
      const transRef = doc(collection(db, COLLECTION));
      transaction.set(transRef, {
        ...transaction,
        createdAt: Timestamp.now(),
        items: items.map(item => ({
          productId: item.product.id,
          productName: item.product.name,
          price: item.product.price,
          quantity: item.quantity,
          subtotal: item.product.price * item.quantity,
        })),
      });
      
      return transRef.id;
    });
    
    return docRef;
  } catch (error) {
    console.error('Error creating transaction:', error);
    throw error;
  }
};

// READ - Get All Transactions
export const getTransactions = async (): Promise<Transaction[]> => {
  const q = query(collection(db, COLLECTION), orderBy('createdAt', 'desc'));
  const snapshot = await getDocs(q);
  return snapshot.docs.map(doc => ({
    id: doc.id,
    ...doc.data(),
  })) as Transaction[];
};

// READ - Get Transaction by ID
export const getTransactionById = async (id: string): Promise<Transaction | null> => {
  const q = query(collection(db, COLLECTION), where('id', '==', id));
  const snapshot = await getDocs(q);
  if (!snapshot.empty) {
    return { id: snapshot.docs[0].id, ...snapshot.docs[0].data() } as Transaction;
  }
  return null;
};

// READ - Get Transactions by Date Range
export const getTransactionsByDate = async (startDate: Date, endDate: Date): Promise<Transaction[]> => {
  const q = query(
    collection(db, COLLECTION),
    where('createdAt', '>=', Timestamp.fromDate(startDate)),
    where('createdAt', '<=', Timestamp.fromDate(endDate)),
    orderBy('createdAt', 'desc')
  );
  const snapshot = await getDocs(q);
  return snapshot.docs.map(doc => ({
    id: doc.id,
    ...doc.data(),
  })) as Transaction[];
};

// READ - Get Today's Transactions
export const getTodayTransactions = async (): Promise<Transaction[]> => {
  const today = new Date();
  today.setHours(0, 0, 0, 0);
  const tomorrow = new Date(today);
  tomorrow.setDate(tomorrow.getDate() + 1);
  
  return await getTransactionsByDate(today, tomorrow);
};