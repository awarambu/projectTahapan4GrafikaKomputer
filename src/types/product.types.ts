export interface Product {
  id: string;
  name: string;
  price: number;
  stock: number;
  category: string;
  description?: string;
  image?: string;
  createdAt: any;
  updatedAt: any;
}

export interface ProductValidation {
  name?: string;
  price?: string;
  stock?: string;
  category?: string;
}