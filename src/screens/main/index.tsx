import { registerRootComponent } from 'expo';
import React from 'react';
import { ProdukProvider } from './context/ProdukContext';
import AppNavigator from './navigation/AppNavigator'; // Sesuaikan jika nama file navigator Anda berbeda

function App() {
  return (
    <ProdukProvider>
      <AppNavigator />
    </ProdukProvider>
  );
}

// Mendaftarkan komponen utama ke Expo
registerRootComponent(App);