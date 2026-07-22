import React, { createContext, useContext, useState, useEffect, useCallback } from 'react';
import {
  Product,
  getProducts,
  addProduct,
  updateProduct,
  deleteProduct,
  getProductById,
  searchProducts,
  validateProduct,
  ProductValidation,
} from '../services/productService';

interface ProductContextType {
  products: Product[];
  loading: boolean;
  error: string | null;
  fetchProducts: () => Promise<void>;
  addProduct: (data: Omit<Product, 'id' | 'createdAt' | 'updatedAt'>) => Promise<{ success: boolean; errors?: ProductValidation; message?: string }>;
  updateProduct: (id: string, data: Partial<Product>) => Promise<{ success: boolean; errors?: ProductValidation; message?: string }>;
  deleteProduct: (id: string) => Promise<{ success: boolean; message?: string }>;
  getProduct: (id: string) => Promise<Product | null>;
  searchProducts: (text: string) => Promise<Product[]>;
  validateProduct: (data: Partial<Product>) => ProductValidation;
}

const ProductContext = createContext<ProductContextType | undefined>(undefined);

export const ProductProvider: React.FC<{ children: React.ReactNode }> = ({ children }) => {
  const [products, setProducts] = useState<Product[]>([]);
  const [loading, setLoading] = useState(false);
  const [error, setError] = useState<string | null>(null);

  const fetchProducts = useCallback(async () => {
    setLoading(true);
    setError(null);
    try {
      const data = await getProducts();
      setProducts(data);
    } catch (err: any) {
      setError(err.message || 'Gagal memuat produk');
    } finally {
      setLoading(false);
    }
  }, []);

  const addNewProduct = async (data: Omit<Product, 'id' | 'createdAt' | 'updatedAt'>) => {
    const errors = validateProduct(data);
    if (Object.keys(errors).length > 0) {
      return { success: false, errors };
    }
    
    setLoading(true);
    try {
      await addProduct(data);
      await fetchProducts();
      return { success: true };
    } catch (err: any) {
      return { success: false, message: err.message || 'Gagal menambah produk' };
    } finally {
      setLoading(false);
    }
  };

  const updateExistingProduct = async (id: string, data: Partial<Product>) => {
    const errors = validateProduct(data);
    if (Object.keys(errors).length > 0) {
      return { success: false, errors };
    }
    
    setLoading(true);
    try {
      await updateProduct(id, data);
      await fetchProducts();
      return { success: true };
    } catch (err: any) {
      return { success: false, message: err.message || 'Gagal update produk' };
    } finally {
      setLoading(false);
    }
  };

  const deleteExistingProduct = async (id: string) => {
    setLoading(true);
    try {
      await deleteProduct(id);
      await fetchProducts();
      return { success: true };
    } catch (err: any) {
      return { success: false, message: err.message || 'Gagal hapus produk' };
    } finally {
      setLoading(false);
    }
  };

  useEffect(() => {
    fetchProducts();
  }, []);

  return (
    <ProductContext.Provider
      value={{
        products,
        loading,
        error,
        fetchProducts,
        addProduct: addNewProduct,
        updateProduct: updateExistingProduct,
        deleteProduct: deleteExistingProduct,
        getProduct: getProductById,
        searchProducts,
        validateProduct,
      }}
    >
      {children}
    </ProductContext.Provider>
  );
};

export const useProduct = () => {
  const context = useContext(ProductContext);
  if (!context) throw new Error('useProduct must be used within ProductProvider');
  return context;
};