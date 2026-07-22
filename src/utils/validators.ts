export const validators = {
  required: (value: any) => {
    if (value === undefined || value === null) return false;
    if (typeof value === 'string') return value.trim().length > 0;
    return true;
  },
  minLength: (value: string, min: number) => value && value.trim().length >= min,
  isNumber: (value: any) => !isNaN(Number(value)) && Number(value) >= 0,
  isPositiveNumber: (value: any) => !isNaN(Number(value)) && Number(value) > 0,
  isInteger: (value: any) => Number.isInteger(Number(value)),
  email: (email: string) => /^[^\s@]+@[^\s@]+\.[^\s@]+$/.test(email),
};