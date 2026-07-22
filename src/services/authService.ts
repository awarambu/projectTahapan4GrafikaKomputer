import * as SQLite from 'expo-sqlite';
const db = SQLite.openDatabaseSync('kasir.db');
export const initDB = () => {
  db.execSync(`
    CREATE TABLE IF NOT EXISTS produk (
      id INTEGER PRIMARY KEY AUTOINCREMENT,
      nama TEXT NOT NULL,
      harga REAL NOT NULL,
      stok INTEGER NOT NULL DEFAULT 0,
      createdAt TEXT,
      updatedAt TEXT
    );
  `);
  db.execSync(`
    CREATE TABLE IF NOT EXISTS transaksi (
      id INTEGER PRIMARY KEY AUTOINCREMENT,
      total REAL NOT NULL,
      metodePembayaran TEXT NOT NULL,
      createdAt TEXT
    );
  `);
  db.execSync(`
    CREATE TABLE IF NOT EXISTS transaksi_item (
      id INTEGER PRIMARY KEY AUTOINCREMENT,
      transaksiId INTEGER NOT NULL,
      produkId INTEGER NOT NULL,
      nama TEXT NOT NULL,
      harga REAL NOT NULL,
      qty INTEGER NOT NULL,
      FOREIGN KEY (transaksiId) REFERENCES transaksi(id),
      FOREIGN KEY (produkId) REFERENCES produk(id)
    );
  `);
};
export default db;
