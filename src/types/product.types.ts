import { Timestamp } from 'firebase/firestore';

export interface Product {
  id: string; // wajib
  name: string;
  category: string;
  price: number;
  stock: number;
  barcode?: string; // opsional
  createdAt?: Timestamp;
  updatedAt?: Timestamp;
}

export interface ProductValidation {
  name?: string;
  price?: string;
  stock?: string;
  category?: string;
}