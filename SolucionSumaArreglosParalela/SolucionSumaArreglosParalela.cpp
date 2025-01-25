/* Paso 1: Creaci�n del proyecto Visual C++ de tipo de aplicaci�n de consola.*/
#include <iostream>

/*
    Paso 2: configuaraci�n para poder usar la librer�a OpenMP.
        2.1 Ir a las propiedades del proyecto.
        2.2 Seleccionar la opci�n de Idioma.
        2.3 Establecer la Compatibilidad con OpenMP.
    
    Luego de los anteriores  pasos, la librer�a OpenMP queda lista para su uso en el proyecto.
*/
#ifdef _OPENMP 
    #include <omp.h>
#endif

/*
    En esta secci�n se definen las constantes de precompilaci�n usando el formato nuevo. 
    Este formato usa 'expresiones constantes'; el formato anteriormente usado se deja en
    comentarios para mantener el hist�rico del cambio.

    #define N       1000
    #define chunk   100
    #define mostrar 10
*/

constexpr auto N       = 1000;  // Cantidad de elementos en los arreglos.
constexpr auto chunk   = 100;  // Tama�o de los pedazos de arreglos que cada hilo va a trabajar. 
constexpr auto mostrar = 10;  // N�mero de elementos a imprimir en el momento de comprobaci�n de los resultados.

/*
    En esta secci�n se da la definici�n de los nombres de las funciones a implementar luego.
*/
void imprimeArreglo(float* d);
float obtenerNumeroAlAzar();

/*
    Esta es la funci�n principal de la aplicaci�n, la que ejecuta todas las acciones para el conseguir el 
    objetivo final de sumar dos arreglos mediante programaci�n paralela.
*/

int main()
{
    /* 
        Se establece una semilla basada en el tiempo actual del sistema que sirva como base para la generaci�n 
        de los n�meros aleatorios. 
    */
    srand((unsigned)time(NULL));

    /*
        Se realiza una comprobaci�n de la correcta operaci�n de la librer�a OpenMP 
        para continuar con las siguientes secciones en ejecuci�n.
    */

    std::cout << "Verificando OpenMP\n";
    #ifdef _OPENMP
        std::cout << "Librer�a OpenMP en su lugar." << std::endl;
    #elif
        std::cout << "Problemas al encontrar la libreria OpenMP" << std::endl;
        return 0;
    #endif
    
    /* 
        Paso 3: Se da la creaci�n y la asignaci�n de arreglos a trabajar.
        Se incorpora una funci�n de ayuda para generar n�meros aleatorios, los cuales le dan un paso extra al
        llenado de los arreglos de datos originales.
        
        'a' y 'b' son los arreglos que representan los sumandos de la cada operaci�n, por su parte 'c' almacenar� 
        los resultados de cada suma individual de los elementos respectivos de 'a' y 'b'.
    */
    
    std::cout << "Sumando Arreglos en Paralelo!\n";

    float a[N], b[N], c[N];
    int i;

    for (i = 0; i < N; i++)
    {
        float random_n = obtenerNumeroAlAzar();
        /*
            A la versi�n original se le multiplica en cada caso 
            por una variable que ser� calculada al azar en cada ciclo de
            este 'for' de asignaci�n de valores, para darle un elemento
            diferenciador a esta versi�n de la implementaci�n original.

            C�lculo Original:
            a[i] = i * 10;
            b[i] = (i + 3) * 3.7;
        */
        a[i] = (i * 10) * random_n;
        b[i] = ((i + 3) * 3.7) * random_n;
    }
    
    /*
        Paso 4: 'for' paralelo mendiante la librer�a OpenMP.

        4.1 Dicha implementaci�n de 'for' paralelo se da gracias a las instrucciones 'pragma' a�adidas por el OpenMP.
        Esta definici�n 'pragma' le indica al CPU que el 'for' usar� el paralelismo.

        4.2 Luego, la siguiente instrucci�n inmediatamente declarada (y unida a la anterior por un s�mbolo '/') indica que
        'a','b' y 'c' van a estar circunscritos en una secci�n de memoria compartida del computador. 
        El argumento final de esta instrucci�n define el tama�o de cada pedazo de arreglo que cada hilo de ejecuci�n va a 
        procesar.
        En seguida, la variable 'i' debe declararse en modo privado, para evitar interposici�n con los ciclos de entre 
        los hilos circundantes.

        4.3 Finalmente, la siguiente instrucci�n inmediata (tambi�n unida por un s�mbolo '\'), define la planificaci�n 
        de las tareas como est�tica, as� como nuevamente indica los tama�os de los pedazos de datos que cada hilo va a
        trabajar por vez.

        4.4 Si bien el 'for' inmediato parece ser una instrucci�n normal, la triada anterior pavimenta el camino para que 
        esta ejecuci�n se haga en paralelo.
    */
   
    int pedazos = chunk;

    #pragma omp parallel for \
    shared(a, b, c, pedazos) private(i) \
    schedule(static, pedazos)
    
    for (i = 0; i < N; i++)
        c[i] = a[i] + b[i];

    /*
        Paso 5: se da la impresi�n de los resultados de los elementos de cada arreglo; para comprobar que los resultados son correctos.
    */
    
    std::cout << "Imprimiendo los primeros " << mostrar << " valores del arreglo a:" << std::endl;
    imprimeArreglo(a);

    std::cout << "Imprimiendo los primeros " << mostrar << " valores del arreglo b: " << std::endl;
    imprimeArreglo(b);

    std::cout << "Imprimiendo los primeros " << mostrar << " valores del arreglo c: " << std::endl;
    imprimeArreglo(c);
}

/*
    Funci�n de ayuda para la impresi�n al terminal del contenido de 
    los arreglos.
*/
void imprimeArreglo(float* d)
{
    for (int x = 0; x < mostrar; x++)
        std::cout << d[x] << " - ";
    std::cout << std::endl;
}

/*
    Funci�n de ayuda para generar n�meros pseudo-aleatorios (en un rango de diez mil).
*/
float obtenerNumeroAlAzar()
{
    float random = 1 + (rand() % 10001);
    return random;
}