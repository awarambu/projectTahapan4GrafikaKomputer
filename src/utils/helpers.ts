export const formatCurrency = (amount: number): string => {
  return `Rp ${amount.toLocaleString('id-ID')}`;
};

export const formatDate = (date: any): string => {
  if (!date) return '-';
  try {
    const d = date.toDate ? date.toDate() : new Date(date);
    return d.toLocaleDateString('id-ID', {
      day: '2-digit',
      month: 'long',
      year: 'numeric',
      hour: '2-digit',
      minute: '2-digit',
    });
  } catch {
    return '-';
  }
};

export const generateReceiptNumber = (): string => {
  const date = new Date();
  const timestamp = date.getTime().toString().slice(-6);
  const random = Math.floor(Math.random() * 1000).toString().padStart(3, '0');
  return `INV-${date.getFullYear()}${String(date.getMonth()+1).padStart(2,'0')}${String(date.getDate()).padStart(2,'0')}-${timestamp}${random}`;
};

export const getInitials = (name: string): string => {
  if (!name) return '';
  const words = name.split(' ');
  if (words.length === 1) return words[0].charAt(0).toUpperCase();
  return (words[0].charAt(0) + words[words.length - 1].charAt(0)).toUpperCase();
};