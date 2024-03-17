typedef string Texto<100>;
struct Persona {
    Texto nombre;
    int   edad;
};
union Resultado switch(int caso) {
    case 1: int n;
    case 2: float x;
    case 3: Texto error;
};
struct Lista {
    int dato;
    Lista *siguiente;
};