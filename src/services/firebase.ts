import { initializeApp } from 'firebase/app';
import {
  getFirestore,
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
  limit,
  Timestamp,
  writeBatch,
  runTransaction,
} from 'firebase/firestore';
import { getStorage } from 'firebase/storage';
import {
  getAuth,
  signInWithEmailAndPassword,
  createUserWithEmailAndPassword,
  signOut,
  onAuthStateChanged,
  User,
} from 'firebase/auth';

// 🔥 GANTI DENGAN KONFIGURASI FIREBASE ANDA 🔥
const firebaseConfig = {
  apiKey: "AIzaSyBQ6FQXlZvP7JXbTlYo3e_pNvj_5GKOQCg",
  authDomain: "kasir-app-1998c.firebaseapp.com",
  projectId: "kasir-app-1998c",
  storageBucket: "kasir-app-1998c.firebasestorage.app",
  messagingSenderId: "123456789",
  appId: "1:123456789:web:abcdef123456",
};

const app = initializeApp(firebaseConfig);

export const db = getFirestore(app);
export const storage = getStorage(app);
export const auth = getAuth(app);

export {
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
  limit,
  Timestamp,
  writeBatch,
  runTransaction,
  signInWithEmailAndPassword,
  createUserWithEmailAndPassword,
  signOut,
  onAuthStateChanged,
  type User,
};