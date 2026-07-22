import React from 'react';
import { createStackNavigator } from '@react-navigation/stack';
import { createBottomTabNavigator } from '@react-navigation/bottom-tabs';
import Icon from 'react-native-vector-icons/Ionicons';

// Auth Screens
import SplashScreen from '../screens/auth/SplashScreen';
import LoginScreen from '../screens/auth/LoginScreen';
import RegisterScreen from '../screens/auth/RegisterScreen';

// Main Screens
import DashboardScreen from '../screens/main/DashboardScreen';
import DaftarProdukScreen from '../screens/main/DaftarProdukScreen';
import DetailProdukScreen from '../screens/main/Detailprodukscreen';
import FormProdukScreen from '../screens/main/FormProdukScreen';
import KeranjangScreen from '../screens/main/KeranjangScreen';
import PilihProdukScreen from '../screens/main/PilihProdukScreen';
import ProfileScreen from '../screens/main/ProfileScreen';
import StrukScreen from '../screens/main/StrukScreen';
import RiwayatTransaksiScreen from '../screens/main/RiwayatTransaksiScreen';

// === TAMBAHKAN SCREEN TAMBAH PRODUK YANG BARU ===
import TambahProdukScreen from '../screens/main/TambahProdukScreen'; // <-- tambahkan ini

import { useCart } from '../context/CartContext';
import { colors } from '../theme/colors';

export type RootStackParamList = {
  Splash: undefined;
  Login: undefined;
  Register: undefined;
  Main: undefined;
  DetailProduk: { productId: string };
  FormProduk: { mode: 'add' | 'edit'; productId?: string };
  TambahProduk: undefined; // <-- tambahkan tipe baru
  Keranjang: undefined;
  PilihProduk: undefined;
  Struk: undefined;
  RiwayatTransaksi: undefined;
};

export type MainTabParamList = {
  Dashboard: undefined;
  Produk: undefined;
  Keranjang: undefined;
  Profile: undefined;
};

const Stack = createStackNavigator<RootStackParamList>();
const Tab = createBottomTabNavigator<MainTabParamList>();

const MainTabs = () => {
  const { getTotalItems } = useCart();
  const totalItems = getTotalItems();

  return (
    <Tab.Navigator
      screenOptions={({ route }) => ({
        tabBarIcon: ({ focused, color, size }) => {
          const iconMap: Record<string, string> = {
            Dashboard: focused ? 'home' : 'home-outline',
            Produk: focused ? 'grid' : 'grid-outline',
            Keranjang: focused ? 'cart' : 'cart-outline',
            Profile: focused ? 'person' : 'person-outline',
          };
          return <Icon name={iconMap[route.name]} size={size} color={color} />;
        },
        tabBarActiveTintColor: colors.primary,
        tabBarInactiveTintColor: colors.textTertiary,
        headerShown: false,
        tabBarStyle: {
          height: 60,
          paddingBottom: 8,
          paddingTop: 8,
          backgroundColor: colors.surface,
          borderTopColor: colors.border,
        },
        tabBarLabelStyle: {
          fontSize: 11,
          fontWeight: '500',
        },
      })}
    >
      <Tab.Screen name="Dashboard" component={DashboardScreen} options={{ title: 'Beranda' }} />
      <Tab.Screen name="Produk" component={DaftarProdukScreen} options={{ title: 'Produk' }} />
      <Tab.Screen
        name="Keranjang"
        component={KeranjangScreen}
        options={{
          title: 'Keranjang',
          tabBarBadge: totalItems > 0 ? totalItems : undefined,
          tabBarBadgeStyle: { backgroundColor: colors.danger },
        }}
      />
      <Tab.Screen name="Profile" component={ProfileScreen} options={{ title: 'Profil' }} />
    </Tab.Navigator>
  );
};

export const AppNavigator = () => {
  return (
    <Stack.Navigator
      initialRouteName="Splash"
      screenOptions={{
        headerShown: false,
        cardStyle: { backgroundColor: colors.background },
      }}
    >
      <Stack.Screen name="Splash" component={SplashScreen} />
      <Stack.Screen name="Login" component={LoginScreen} />
      <Stack.Screen name="Register" component={RegisterScreen} />
      <Stack.Screen name="Main" component={MainTabs} />

      <Stack.Screen
        name="DetailProduk"
        component={DetailProdukScreen}
        options={{
          headerShown: true,
          headerTitle: 'Detail Produk',
          headerBackTitle: 'Kembali',
        }}
      />

      {/* Screen FormProduk yang sudah ada (jika masih dipakai) */}
      <Stack.Screen
        name="FormProduk"
        component={FormProdukScreen}
        options={{
          headerShown: true,
          headerTitle: 'Form Produk',
          headerBackTitle: 'Kembali',
        }}
      />

      {/* === SCREEN TAMBAH PRODUK BARU === */}
      <Stack.Screen
        name="TambahProduk"
        component={TambahProdukScreen}
        options={{
          headerShown: true,
          headerTitle: 'Tambah Produk',
          headerBackTitle: 'Kembali',
        }}
      />

      <Stack.Screen
        name="PilihProduk"
        component={PilihProdukScreen}
        options={{
          headerShown: true,
          headerTitle: 'Pilih Produk',
          headerBackTitle: 'Kembali',
        }}
      />
      <Stack.Screen
        name="Struk"
        component={StrukScreen}
        options={{
          headerShown: true,
          headerTitle: 'Struk Transaksi',
          headerBackTitle: 'Kembali',
        }}
      />
      <Stack.Screen
        name="RiwayatTransaksi"
        component={RiwayatTransaksiScreen}
        options={{
          headerShown: true,
          headerTitle: 'Riwayat Transaksi',
          headerBackTitle: 'Kembali',
        }}
      />
    </Stack.Navigator>
  );
};