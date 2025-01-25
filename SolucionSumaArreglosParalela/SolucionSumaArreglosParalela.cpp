/* Paso 1: Creación del proyecto Visual C++ de tipo de aplicación de consola.*/
#include <iostream>

/*
    Paso 2: configuaración para poder usar la librería OpenMP.
        2.1 Ir a las propiedades del proyecto.
        2.2 Seleccionar la opción de Idioma.
        2.3 Establecer la Compatibilidad con OpenMP.
    
    Luego de los anteriores  pasos, la librería OpenMP queda lista para su uso en el proyecto.
*/
#ifdef _OPENMP 
    #include <omp.h>
#endif

/*
    En esta sección se definen las constantes de precompilación usando el formato nuevo. 
    Este formato usa 'expresiones constantes'; el formato anteriormente usado se deja en
    comentarios para mantener el histórico del cambio.

    #define N       1000
    #define chunk   100
    #define mostrar 10
*/

constexpr auto N       = 1000;  // Cantidad de elementos en los arreglos.
constexpr auto chunk   = 100;  // Tamaño de los pedazos de arreglos que cada hilo va a trabajar. 
constexpr auto mostrar = 10;  // Número de elementos a imprimir en el momento de comprobación de los resultados.

/*
    En esta sección se da la definición de los nombres de las funciones a implementar luego.
*/
void imprimeArreglo(float* d);
float obtenerNumeroAlAzar();

/*
    Esta es la función principal de la aplicación, la que ejecuta todas las acciones para el conseguir el 
    objetivo final de sumar dos arreglos mediante programación paralela.
*/

int main()
{
    /* 
        Se establece una semilla basada en el tiempo actual del sistema que sirva como base para la generación 
        de los números aleatorios. 
    */
    srand((unsigned)time(NULL));

    /*
        Se realiza una comprobación de la correcta operación de la librería OpenMP 
        para continuar con las siguientes secciones en ejecución.
    */

    std::cout << "Verificando OpenMP\n";
    #ifdef _OPENMP
        std::cout << "Librería OpenMP en su lugar." << std::endl;
    #elif
        std::cout << "Problemas al encontrar la libreria OpenMP" << std::endl;
        return 0;
    #endif
    
    /* 
        Paso 3: Se da la creación y la asignación de arreglos a trabajar.
        Se incorpora una función de ayuda para generar números aleatorios, los cuales le dan un paso extra al
        llenado de los arreglos de datos originales.
        
        'a' y 'b' son los arreglos que representan los sumandos de la cada operación, por su parte 'c' almacenará 
        los resultados de cada suma individual de los elementos respectivos de 'a' y 'b'.
    */
    
    std::cout << "Sumando Arreglos en Paralelo!\n";

    float a[N], b[N], c[N];
    int i;

    for (i = 0; i < N; i++)
    {
        float random_n = obtenerNumeroAlAzar();
        /*
            A la versión original se le multiplica en cada caso 
            por una variable que será calculada al azar en cada ciclo de
            este 'for' de asignación de valores, para darle un elemento
            diferenciador a esta versión de la implementación original.

            Cálculo Original:
            a[i] = i * 10;
            b[i] = (i + 3) * 3.7;
        */
        a[i] = (i * 10) * random_n;
        b[i] = ((i + 3) * 3.7) * random_n;
    }
    
    /*
        Paso 4: 'for' paralelo mendiante la librería OpenMP.

        4.1 Dicha implementación de 'for' paralelo se da gracias a las instrucciones 'pragma' añadidas por el OpenMP.
        Esta definición 'pragma' le indica al CPU que el 'for' usará el paralelismo.

        4.2 Luego, la siguiente instrucción inmediatamente declarada (y unida a la anterior por un símbolo '/') indica que
        'a','b' y 'c' van a estar circunscritos en una sección de memoria compartida del computador. 
        El argumento final de esta instrucción define el tamaño de cada pedazo de arreglo que cada hilo de ejecución va a 
        procesar.
        En seguida, la variable 'i' debe declararse en modo privado, para evitar interposición con los ciclos de entre 
        los hilos circundantes.

        4.3 Finalmente, la siguiente instrucción inmediata (también unida por un símbolo '\'), define la planificación 
        de las tareas como estática, así como nuevamente indica los tamaños de los pedazos de datos que cada hilo va a
        trabajar por vez.

        4.4 Si bien el 'for' inmediato parece ser una instrucción normal, la triada anterior pavimenta el camino para que 
        esta ejecución se haga en paralelo.
    */
   
    int pedazos = chunk;

    #pragma omp parallel for \
    shared(a, b, c, pedazos) private(i) \
    schedule(static, pedazos)
    
    for (i = 0; i < N; i++)
        c[i] = a[i] + b[i];

    /*
        Paso 5: se da la impresión de los resultados de los elementos de cada arreglo; para comprobar que los resultados son correctos.
    */
    
    std::cout << "Imprimiendo los primeros " << mostrar << " valores del arreglo a:" << std::endl;
    imprimeArreglo(a);

    std::cout << "Imprimiendo los primeros " << mostrar << " valores del arreglo b: " << std::endl;
    imprimeArreglo(b);

    std::cout << "Imprimiendo los primeros " << mostrar << " valores del arreglo c: " << std::endl;
    imprimeArreglo(c);
}

/*
    Función de ayuda para la impresión al terminal del contenido de 
    los arreglos.
*/
void imprimeArreglo(float* d)
{
    for (int x = 0; x < mostrar; x++)
        std::cout << d[x] << " - ";
    std::cout << std::endl;
}

/*
    Función de ayuda para generar números pseudo-aleatorios (en un rango de diez mil).
*/
float obtenerNumeroAlAzar()
{
    float random = 1 + (rand() % 10001);
    return random;
}