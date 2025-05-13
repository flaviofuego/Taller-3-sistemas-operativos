# Usar una imagen base de Ubuntu
FROM ubuntu:20.04

# Evitar interacciones durante la instalación de paquetes
ENV DEBIAN_FRONTEND=noninteractive

# Actualizar repositorios e instalar herramientas necesarias
RUN apt-get update && apt-get install -y \
    build-essential \
    procps \
    gcc \
    make \
    vim \
    nano \
    valgrind \
    && rm -rf /var/lib/apt/lists/*

# Establecer el directorio de trabajo
WORKDIR /app

# Copiar los archivos del proyecto al contenedor
COPY . /app

# Hacer ejecutable el script de entrada
RUN chmod +x /app/entrypoint.sh
RUN chmod +x recompile.sh


# Usar el script como punto de entrada
ENTRYPOINT ["/app/entrypoint.sh"]