import { Timestamp } from 'firebase/firestore';
import { Product } from './product.types';

export interface CartItem {
  product: Product;
  quantity: number;
}

export interface Transaction {
  id?: string;
  receiptNumber: string;
  items: CartItem[];
  subtotal: number;
  tax: number;
  total: number;
  paymentMethod: string;
  paymentAmount: number;
  change: number;
  cashierName: string;
  createdAt?: Timestamp;
}

export interface TransactionHistory extends Transaction {
  id: string;
}