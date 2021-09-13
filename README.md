# TP-1-SO    -    README

<hr>

## Autores

- [Alberto Abancens] (https://github.com/tataabancens)
- [Gonzalo Rossin] (https://github.com/GonzaloRossin)
- [Uriel Mihura] (https://github.com/uri-99)

## Aplicación

Esta es una aplicación de resolución de problemas **SAT**, y consiste de los siguientes archivos:

  + **master**: crea los esclavos, les asigna archivos para resolver y envía las resoluciones a **view** y **result.txt**.
  + **slave**: realiza el la resolución de los problemas enviados por **master** utilizando **minisat**.
  + **view**: imprime los datos por *standard output*.
  + **shared_memory**: librería utilizada para el manejo de memoria compartida, con sus respectivos semaforos.

## Compilación

  Instalar **minisat**, `apt-get install minisat`.
  Ejecutar `make` o `make all` para compilar los archivos utilizando el **Makefile**.
  La compilación genera los archivos:

+ **Master**
+ **Slave**
+ **View**

Para remover todo lo generado en compilación, puede ejecutar `make clean`

## Ejecución

  Para ejecutar el programa, se debe correr el archivo **Master**, pasandole por parámetro los archivos .cnf a resolver:

  ```bash
    ./Master ./files/*
  ```

  Una vez ejecutado el **Master**, puede optar por correr también el **View** desde otra terminal, pasandole como argumento el valor que imprimió **Master** cuando fue ejecutado:

  ```bash
  # Terminal 1:
    ./Master ./files/*
  4
  ```

  ```bash
  # Terminal 2:
   ./View 4
  ```

  Alternativamente, se puede *pipear* la salida de **Master** a **View**:
  
  ```bash
  ./Master ./files/* | ./View
  ```

  Al terminar la ejecución de **Master**, éste creará un archivo, **result.txt**, conteniendo la resolución de los **SAT**.

## Testeo


  Para el testeo con **Valgrind**, deberá primero verificar que esté correctamente instalado con el comando `apt-get install valgrind`. Es muy probable que ya lo tenga.
  Luego deberá agregar el *flag* -g al comando de compilación. Puede también agregar el *flag* -O0 para un testeo mas completo, pero cuidado, ten en cuenta que éste puede realentizar todo por un factor de casi 20 veces.
   
   `valgrind --leak-check=yes ./Master ./files/*`

   La salida de este comando le informará si **Valgrind** pudo encontrar *memory leaks*

   Para el testeo con **PVS-Studio** deberá primero instalarlo por línea de comando, y en este caso podrá utilizar las credenciales gratuitas que ofrece PVS-Studio ya que este es un proyecto acádemico universitario sin fines de lucro.

  ```bash
  apt-get update
  apt-get install pvs-studio
  pvs-studio-analyzer credentials "PVS-Studio Free" "FREE-FREE-FREE-FREE"
  ```

  Luego deberá agregar las siguientes 2 líneas comentadas al principio de todos los archivos .c que serán analizados:

  ```bash
  // This is a personal academic project. Dear PVS-Studio, please check it.
  // PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
  ```

  Luego es muy recomendable instalar y utilizar la herramienta **Bear** para correctamente configurar las instrucciones de compilación que utilizará el **PVS-Studio**

  ```bash
  apt--get install bear
  bear -- make
  ```

  Y finalmente podrá ejecutar el **PVS-Studio**. Este anda mejor si realiza un `make clean` antes de ejecutarse:

  ``pvs-studio-analizer analyze -o project.log``

  Este generará un informe con el nombre **project.log**, pero para que sea legible por el usuario cosidere:

  ``plog-converter -a GA:1,2 -t tasklist -o project.tasks project.log``

  Ahora con un simple ``cat project.tasks`` podrá visualizar los resultados del testeo con **PVS-Studio**

  Para el testeo con **Cppcheck**, deberá primero instalarlo ``apt-get install cppcheck``

  Luego puede simplemente ejecutarlo, pasando el directorio contenedor del projecto como parametro, y opcionalmente puede agregarle el flag ``--enable=all`` para que éste haga un chequeo mas a fondo.:

  ``cppcheck --enanle=all ./``

  Este recursivamente chequeará todos los archivos, y escribirá en pantalla si encuentra algún error.

   Nota: si no imprime nada y acaba solamente con un *done*, el chequeo fue satisfactorio,












