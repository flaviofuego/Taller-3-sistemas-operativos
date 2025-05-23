#!/bin/bash

echo "🔄 Recompilando todos los programas del taller..."
echo "------------------------------------------------"

# Función para compilar y mostrar resultados
compile_program() {
    local source=$1
    local output=$2
    echo -n "Compilando $source... "
    gcc -Wall -o $output $source
    
    if [ $? -eq 0 ]; then
        echo "✅ Compilación exitosa"
    else
        echo "❌ Error de compilación"
        return 1
    fi
    return 0
}

# Limpiar los ejecutables anteriores
rm -f notas_malloc_indices notas_malloc_punteros notas_calloc_indices notas_calloc_punteros

# Compilar cada programa
errors=0
compile_program notas_malloc_indices.c notas_malloc_indices || ((errors++))
compile_program notas_malloc_punteros.c notas_malloc_punteros || ((errors++))
compile_program notas_calloc_indices.c notas_calloc_indices || ((errors++))
compile_program notas_calloc_punteros.c notas_calloc_punteros || ((errors++))

echo "------------------------------------------------"
if [ $errors -eq 0 ]; then
    echo "✨ Todos los programas se compilaron correctamente"
    echo "📋 Programas disponibles para ejecutar:"
    echo "  ./notas_malloc_indices"
    echo "  ./notas_malloc_punteros"
    echo "  ./notas_calloc_indices"
    echo "  ./notas_calloc_punteros"
else
    echo "⚠️ Algunos programas no se pudieron compilar ($errors errores)"
fi