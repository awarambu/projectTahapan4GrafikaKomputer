// src/screens/main/index.tsx
import React from 'react';
import { ProductProvider } from '../../context/ProdukContext';
import { AppNavigator } from '../../navigation/AppNavigator';   // <-- pakai kurung kurawal

function MainApp() {
  return (
    <ProductProvider>
      <AppNavigator />
    </ProductProvider>
  );
}

export default MainApp;