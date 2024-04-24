#!/bin/bash

# Directorio donde se encuentran los archivos JAR
JARS_DIR="./Jars"

# Nombre de los archivos JAR requeridos
AMQP_CLIENT_JAR="amqp-client-5.21.0.jar"
SLF4J_API_JAR="slf4j-api-1.7.2.jar"
SLF4J_SIMPLE_JAR="slf4j-simple-1.7.2.jar"

# Ruta completa de los archivos JAR
AMQP_CLIENT_PATH="$JARS_DIR/$AMQP_CLIENT_JAR"
SLF4J_API_PATH="$JARS_DIR/$SLF4J_API_JAR"
SLF4J_SIMPLE_PATH="$JARS_DIR/$SLF4J_SIMPLE_JAR"

# Directorio donde se encuentran los archivos fuente
SOURCE_DIR="./"

# Nombre del archivo fuente
SOURCE_FILE="Worker.java"

# Directorio de salida para los archivos compilados
OUTPUT_DIR="./"

# Nombre de la clase principal
MAIN_CLASS="Worker"

# Combinar las rutas de los archivos JAR en una sola variable CLASSPATH
CLASSPATH="$AMQP_CLIENT_PATH:$SLF4J_API_PATH:$SLF4J_SIMPLE_PATH:."

# Compilar la clase Java
javac -cp "$CLASSPATH" -d "$OUTPUT_DIR" "$SOURCE_DIR/$SOURCE_FILE"

# Ejecutamos la clase
java -cp "$CLASSPATH" "$SOURCE_DIR/$SOURCE_FILE"