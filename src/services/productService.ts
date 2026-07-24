// src/services/productService.ts
import {
  db,
  collection,
  addDoc,
  doc,
  getDoc,
  getDocs,
  updateDoc,
  deleteDoc,
  query,
  orderBy,
  where,
  Timestamp,
} from './firebase';
import { Product, ProductValidation } from '../types/product.types';

const COLLECTION = 'products';

// ✅ EXPORT DATA DEFAULT
export const defaultProducts = [
  { name: 'Indomie Goreng', category: 'Makanan', price: 3500, stock: 100 },
  { name: 'Aqua 600 ml', category: 'Minuman', price: 4000, stock: 80 },
  { name: 'Teh Botol Sosro', category: 'Minuman', price: 5000, stock: 60 },
  { name: 'Beras Ramos 5 Kg', category: 'Sembako', price: 78000, stock: 25 },
  { name: 'Gula Pasir 1 Kg', category: 'Sembako', price: 18000, stock: 40 },
  { name: 'Minyak Goreng Bimoli 1 L', category: 'Sembako', price: 23000, stock: 35 },
  { name: 'Telur Ayam 1 Kg', category: 'Sembako', price: 32000, stock: 20 },
  { name: 'Kopi Kapal Api', category: 'Minuman', price: 2500, stock: 120 },
  { name: 'Susu Ultra 1 L', category: 'Minuman', price: 19000, stock: 30 },
  { name: 'Roti Tawar', category: 'Makanan', price: 16000, stock: 25 },
];

// ========== CREATE ==========
export const addProduct = async (product: Omit<Product, 'id' | 'createdAt' | 'updatedAt'>): Promise<string> => {
  const docRef = await addDoc(collection(db, COLLECTION), {
    ...product,
    createdAt: Timestamp.now(),
    updatedAt: Timestamp.now(),
  });
  return docRef.id;
};

// ========== READ ==========
export const getProducts = async (): Promise<Product[]> => {
  const q = query(collection(db, COLLECTION), orderBy('createdAt', 'desc'));
  const snapshot = await getDocs(q);
  return snapshot.docs.map((doc) => ({ id: doc.id, ...doc.data() } as Product));
};

export const getProductById = async (id: string): Promise<Product | null> => {
  const docRef = doc(db, COLLECTION, id);
  const snapshot = await getDoc(docRef);
  return snapshot.exists() ? ({ id: snapshot.id, ...snapshot.data() } as Product) : null;
};

// ========== UPDATE ==========
export const updateProduct = async (id: string, data: Partial<Product>): Promise<void> => {
  await updateDoc(doc(db, COLLECTION, id), { ...data, updatedAt: Timestamp.now() });
};

// ========== DELETE ==========
export const deleteProduct = async (id: string): Promise<void> => {
  await deleteDoc(doc(db, COLLECTION, id));
};

// ========== VALIDATE ==========
export const validateProduct = (product: Partial<Product>): ProductValidation => {
  const errors: ProductValidation = {};
  if (!product.name?.trim()) errors.name = 'Nama produk wajib diisi';
  else if (product.name.trim().length < 3) errors.name = 'Nama produk minimal 3 karakter';
  if (product.price === undefined || product.price === null) errors.price = 'Harga wajib diisi';
  else if (product.price <= 0) errors.price = 'Harga harus lebih dari 0';
  else if (!Number.isInteger(product.price)) errors.price = 'Harga harus angka bulat';
  if (product.stock === undefined || product.stock === null) errors.stock = 'Stok wajib diisi';
  else if (product.stock < 0) errors.stock = 'Stok tidak boleh negatif';
  else if (!Number.isInteger(product.stock)) errors.stock = 'Stok harus angka bulat';
  if (!product.category?.trim()) errors.category = 'Kategori wajib diisi';
  return errors;
};