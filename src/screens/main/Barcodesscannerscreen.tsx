import React, { useState, useEffect } from 'react';
import { View, Text, StyleSheet, TouchableOpacity } from 'react-native';
import { CameraView, useCameraPermissions, BarcodeScanningResult } from 'expo-camera';

/**
 * Screen untuk scan barcode/QR produk.
 *
 * Cara pakai (dari screen lain, misal DaftarProdukScreen):
 * navigation.navigate('BarcodeScanner', {
 *   onScanned: (code: string) => {
 *     // cari produk berdasarkan barcode, lalu tambahkan ke keranjang
 *   },
 * });
 */
type Props = {
  navigation: any;
  route: {
    params: {
      onScanned: (code: string) => void;
    };
  };
};

export default function BarcodeScannerScreen({ navigation, route }: Props) {
  const [permission, requestPermission] = useCameraPermissions();
  const [isProcessing, setIsProcessing] = useState(false);

  useEffect(() => {
    if (!permission?.granted) {
      requestPermission();
    }
  }, [permission]);

  const handleBarcodeScanned = (result: BarcodeScanningResult) => {
    if (isProcessing) return; // mencegah scan ganda dalam waktu singkat

    setIsProcessing(true);
    route.params.onScanned(result.data);
    navigation.goBack();
  };

  if (!permission) {
    return <View style={styles.center} />;
  }

  if (!permission.granted) {
    return (
      <View style={styles.center}>
        <Text style={styles.permissionText}>
          Aplikasi butuh izin kamera untuk scan barcode
        </Text>
        <TouchableOpacity style={styles.permissionButton} onPress={requestPermission}>
          <Text style={styles.permissionButtonText}>Izinkan Kamera</Text>
        </TouchableOpacity>
      </View>
    );
  }

  return (
    <View style={styles.container}>
      <CameraView
        style={StyleSheet.absoluteFillObject}
        facing="back"
        barcodeScannerSettings={{
          barcodeTypes: ['qr', 'ean13', 'ean8', 'code128', 'code39'],
        }}
        onBarcodeScanned={isProcessing ? undefined : handleBarcodeScanned}
      />
      <View style={styles.overlay}>
        <View style={styles.scanBox} />
        <Text style={styles.hintText}>Arahkan kamera ke barcode produk</Text>
      </View>
    </View>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    backgroundColor: 'black',
  },
  center: {
    flex: 1,
    justifyContent: 'center',
    alignItems: 'center',
    padding: 24,
  },
  overlay: {
    flex: 1,
    justifyContent: 'center',
    alignItems: 'center',
  },
  scanBox: {
    width: 250,
    height: 250,
    borderWidth: 2,
    borderColor: '#00e676',
    borderRadius: 12,
  },
  hintText: {
    color: 'white',
    fontSize: 16,
    marginTop: 24,
  },
  permissionText: {
    textAlign: 'center',
    marginBottom: 16,
    color: '#2d3436',
  },
  permissionButton: {
    backgroundColor: '#0984e3',
    paddingVertical: 10,
    paddingHorizontal: 20,
    borderRadius: 8,
  },
  permissionButtonText: {
    color: 'white',
    fontWeight: '600',
  },
});