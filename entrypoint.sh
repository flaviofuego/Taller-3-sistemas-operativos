#!/bin/bash

echo "🔨 Compilando programas automáticamente..."

gcc -o notas_malloc_indices notas_malloc_indices.c 2>/dev/null && echo "✅ notas_malloc_indices compilado" || echo "⚠️ Error al compilar notas_malloc_indices.c"
gcc -o notas_malloc_punteros notas_malloc_punteros.c 2>/dev/null && echo "✅ notas_malloc_punteros compilado" || echo "⚠️ Error al compilar notas_malloc_punteros.c"
gcc -o notas_calloc_indices notas_calloc_indices.c 2>/dev/null && echo "✅ notas_calloc_indices compilado" || echo "⚠️ Error al compilar notas_calloc_indices.c"
gcc -o notas_calloc_punteros notas_calloc_punteros.c 2>/dev/null && echo "✅ notas_calloc_punteros compilado" || echo "⚠️ Error al compilar notas_calloc_punteros.c"

echo ""
echo "✨ Compilación completada."
echo "📝 Archivo de entrada disponible: notas.txt"
echo ""
echo "Para ejecutar los programas:"
echo "  ./notas_malloc_indices"
echo "  ./notas_malloc_punteros"
echo "  ./notas_calloc_indices"
echo "  ./notas_calloc_punteros"
echo ""
echo "Para verificar fugas de memoria:"
echo "  valgrind ./notas_malloc_indices"
echo ""
echo "📁 Archivos disponibles:"
ls -la
echo ""
echo "🚀 Iniciando shell interactiva..."

# Iniciar shell interactiva
exec /bin/bash