#include <iostream>
#include <stdio.h>
#include<conio.h>
#include<string.h>
#include<stdlib.h> 

#define NOMBRE_ARCHIVO "Asegurados.BAK"
#define ARCHIVO_PROCESAR "Procesados.BAK"

using namespace std;

struct registroIncidente
{
	int codigoIncidente;
	char fechaHora[14]; //AAAAMMDDHH:MM
	char dniAsegurado[11];
	char dniOtroConductor[11];
	int nro_poliza;
	char calle[20];
	int altura;
};
struct NodoIncidente{
  registroIncidente Incidente;
  NodoIncidente *sgte;
};
struct registroAsegurado
{
  int nro_poliza;
  char dniAsegurado[11];
  char nombre[50];
  char apellido[50];
  bool cuota=true;
  char patente[50];
  bool activa=false; 
  int cant_incidentes = 0;
};
struct Nodo{ 
    registroAsegurado Poliza;
    Nodo *sgte;
    NodoIncidente *sublista;
};

void AgregarElemento2( NodoIncidente *&lista2, registroIncidente elemento){
    NodoIncidente *aux=lista2;
    while (aux!=NULL && aux->sgte!=NULL)
    {
        aux=aux->sgte;
    }
    if (aux!=NULL)
    {
        aux->sgte=new NodoIncidente();
        aux=aux->sgte;
        aux->Incidente= elemento;
        aux->sgte=NULL;
    }
    else
    {
        lista2=new NodoIncidente();
        lista2->Incidente=elemento;
        lista2->sgte=NULL;
    }
    return;
}

void agregarElemento(Nodo *&lista, registroAsegurado elemento)
{
    Nodo *aux=lista;
    while (aux!=NULL && aux->sgte!=NULL)
    {
        aux=aux->sgte;
    }
    if (aux!=NULL)
    {
        aux->sgte=new Nodo();
        aux=aux->sgte;
        aux->Poliza=elemento;
        aux->Poliza.cant_incidentes = 0;
        aux->sgte=NULL;
    }
    else
    {
        lista=new Nodo();
        lista->Poliza=elemento;
        lista->Poliza.cant_incidentes = 0;
        lista->sgte=NULL;
    }
    return;
}
void levantar_cuentas(Nodo *&lista) 
{
    FILE *f, *i;
    registroAsegurado buffer;
    registroIncidente buffer2;
    Nodo *aux;

    if (f=fopen(NOMBRE_ARCHIVO,"rb"))
    {
        
        while(fread(&buffer,sizeof(registroAsegurado),1,f))
        {
            agregarElemento(lista,buffer);
            
            if(i = fopen(ARCHIVO_PROCESAR,"rb")){
                while( fread(&buffer2,sizeof(registroIncidente), 1, i)){
                    aux = lista;
                  while(aux){ 
                    if(aux->Poliza.nro_poliza == buffer2.nro_poliza){
                        aux->Poliza.cant_incidentes ++;
                      AgregarElemento2(lista->sublista, buffer2);
                      
                    }
                        aux = aux->sgte;
                    
                  }
                }
                 fclose(i);
            } else {
                cout << "No se pude abrir el archivo " << ARCHIVO_PROCESAR << " " << endl;
            }
            
        }
        fclose(f);
       
        cout << "Lista cargada exitosamente! " << endl;
    }
    else
    {
        cout << "No se pude abrir el archivo " << NOMBRE_ARCHIVO << " " << endl;
    }
    return;
}

int contarLista (Nodo *lista)
{
    Nodo *aux=lista;
    int i=0;
    while(aux)
    {
        i++;
        aux=aux->sgte;
    }
    return i;
}
Nodo* buscarAnterior(Nodo *lista, Nodo *aux)
{
    Nodo *aux1=NULL;
    if (lista!=aux)
    {
        aux1= lista;
        while (aux1 && (aux1->sgte!=aux) )
        {
            aux1=aux1->sgte;
        }
        return aux1;
    }
    else
        return NULL;
}
void ordenar_polizas(Nodo *&lista)
{
    int n,i,j;
    Nodo *aux;
    Nodo *anterior;
    Nodo *proximo;
    bool intercambio;
    n=contarLista(lista);
    for (i=0;i<=n-2;i++)
    {
        aux=lista;
        for (j=0; j<=n-2-i; j++)
        {
            if (aux->sgte->Poliza.cant_incidentes < aux->Poliza.cant_incidentes)
            {
                intercambio=true;
                if (aux==lista)
                {
                    lista= aux->sgte;
                    proximo= lista->sgte;
                    lista->sgte=aux;
                    aux->sgte=proximo;
                }
                else
                {
                    anterior=buscarAnterior(lista,aux);
                    proximo=aux->sgte;
                    anterior->sgte= proximo;
                    aux->sgte= proximo->sgte;
                    proximo->sgte= aux;
                }
            }
            else
                intercambio=false;
            if (!intercambio)
            {
                aux=aux->sgte;
            }
        }
    }
    return;
}

void mostrarPoliza(registroAsegurado pol){
  cout << "------Poliza nro: " << pol.nro_poliza << " ----------" << endl;
  cout << "DNI: " << pol.dniAsegurado << endl;
  cout << "Nombre y apellido: " <<  pol.nombre << " " << pol.apellido << endl;
  cout << "Estado de la cuota: ";
  if (pol.cuota) {
    cout << "Al dia" << endl;
  }
  else {
    cout << "Presenta deudas" << endl;
  }
    cout << "Patente: " << pol.patente << endl;
    cout << "Cantidad de incidentes acumulados: " << pol.cant_incidentes << endl;
    cout << "Estado de poliza: ";
    if(pol.activa){
      cout << "Activa" << endl;
    } else {
      cout << "Desactivada" << endl;
    }
}

void listar_polizas(Nodo *lista){
    Nodo *aux=lista;
    if( lista){
      cout << "Listando polizas" << endl;
      while (aux!=NULL)
      {
          if(aux->Poliza.activa)
          {
             mostrarPoliza(aux->Poliza);
          }
          aux=aux->sgte;
      }
    } else {
      cout << "Lista vacia !!" << endl;
    }
    return;
}
void limpirarListaIns(NodoIncidente *&lista){
  NodoIncidente *anterior;

  while(lista){
    anterior = lista;
    if(anterior->sgte != NULL){
      lista = lista=lista->sgte;
      delete anterior;
    } else {
      delete lista;
      lista = NULL;
    }
  }
}
void  limpiarListas(Nodo *&lista){
    Nodo *anterior;
    NodoIncidente *incAnt;
    while(lista){
        anterior = lista;
        incAnt = lista->sublista;
        if(anterior->sgte != NULL){
            limpirarListaIns(incAnt);
            lista = lista->sgte;
            delete anterior;
        } else {
            delete lista;
            lista = NULL;
        }
    }

}


void desactivar_poliza(int npol, Nodo *&asegurados){
  Nodo *aux = asegurados;
  while(aux && (aux->Poliza.nro_poliza != npol)){
      aux = aux->sgte;
  }
  if(aux){
    aux->Poliza.activa = false;
    cout << "Poliza desactivada!!" << endl;
  } else {
    cout << "Esa poliza no existe !!" << endl;
  }
}

void cargar_pol(Nodo *&lista){
    
    registroAsegurado pol;
    char o;

    cout << "Ingrese el número de póliza" << endl;
    cin >> pol.nro_poliza;
    cout << "Ingrese el dni: " << endl;
    cin >> pol.dniAsegurado; 
    cout << "Ingrese el Nombre:" << endl;
    cin >> pol.nombre;
    cout << "Ingrese el Apellido: " << endl;
    cin >> pol.apellido; 
    
    do {
    cout << "Esta la cuota al dia? (s / n) " << endl;
    o = getch();
    }while(o != 's' && o !='n' && o != 'S' && o !='N');

    if(o == 's' || o == 'S'){
        pol.cuota = true;
    } else {
        pol.cuota = false;
    }

    cout << "Ingrese la patente del auto: " << endl;
    cin >> pol.patente;
        
    pol.activa  = true;
    cout << " ------------ "<< endl;

    agregarElemento(lista,pol);

    return;
}

void finalizar_jornada(Nodo *&lista , NodoIncidente *&incidente){
      FILE *f;
    registroAsegurado pol;
    if(f=fopen(NOMBRE_ARCHIVO, "wb")){
        while(lista){
          pol = lista->Poliza;
          //si esta desactivada no se guarda
          if(pol.activa){
            fwrite(&pol, sizeof(registroAsegurado), 1, f);
          }
          lista = lista->sgte;
        }

    } else {
        cout << "No se pudo actualizar el archivo " << NOMBRE_ARCHIVO << endl;
    }

}

Nodo *busqueda_sec ( int pol_busc ,char dni_busc[], Nodo *lista){

  if(strcmp(dni_busc, "") != 0 ){
    while (lista && strcmp(lista->Poliza.dniAsegurado, dni_busc) != 0){
      lista = lista->sgte;
    }
    if(lista){
      return lista;
    } else {
      cout << "No se encuentro la poliza solicitada !!" << endl;
      return NULL;
    }

  } else if( pol_busc != 0 ){
    while (lista && lista->Poliza.nro_poliza != pol_busc){
      lista = lista->sgte;
    }
    if(lista){
      return lista;
    } else {
      cout << "No se encuentro la poliza solicitada !!" << endl;
      return NULL;
    }
  } else {
    cout << "Nro de poliza o DNI mal invalidos !!" << endl;
    return NULL;
  }
}

void buscar_poliza(Nodo *&lista){
    Nodo *aux;
  
  char dni_buscado[11];
  int poliza_buscado;
  char opcion;

  do {
    cout << "Seleccione una opcion " << endl;
    cout << "a) Buscar por poliza "<< endl;
    cout << "b) Buscar por dni "<< endl;
    opcion = getch(); 
  } while (opcion!='A' && opcion!='B' && opcion!='a' && opcion !='b');

    switch(opcion){
      case 'a': case 'A': 
        cout << "Ingrese numero de la poliza: " << endl;
        cin >> poliza_buscado;

        aux = busqueda_sec(poliza_buscado, {""}, lista);
        if (aux){
            
            cout << "Poliza encontrada :" << endl;
            mostrarPoliza(aux->Poliza);
        } 
      break;
      
      case 'b': case 'B':
        cout << "Ingrese dni: "<< endl;
        cin >> dni_buscado;
        aux = busqueda_sec(0 ,dni_buscado , lista);

        if (aux){
            mostrarPoliza(aux->Poliza);
        } 
        break;
      default: break;
    }
}


void cargar_indicentes() 
{
    FILE*f;
    registroIncidente incidente;
    cout << "-------INCIDENTE-------" << endl;
    cout << "Ingrese la fecha y hora (AAAAMMDDHH:MM)" << endl;
    cin >> incidente.fechaHora;
    cout << "Ingrese dni asegurado" << endl;
    cin >> incidente.dniAsegurado;
    cout << "Ingrese dni del otro conductor" << endl;
    cin >> incidente.dniOtroConductor;
    cout << "Ingrese nro de poliza" << endl;
    cin >> incidente.nro_poliza;
    cout << "Ingrese la calle" << endl;
    cin >> incidente.calle;
    cout << "Ingrese la altura" << endl;
    cin >> incidente.altura;
    if (f=fopen(ARCHIVO_PROCESAR,"ab"))
    {
        fwrite(&incidente,sizeof(registroIncidente),1,f);
        fclose(f);
    }
    return;
}
void procesar_incidentes(Nodo *lista)
{
    Nodo *paux=lista;
    NodoIncidente *aux;
    FILE *f;
    registroIncidente Incidente;
    int cant;
    
    if(paux){
        if (f = fopen(ARCHIVO_PROCESAR,"rb")){
            while(fread(&Incidente,sizeof(registroIncidente),1,f)){
                while(paux && (paux->Poliza.nro_poliza != Incidente.nro_poliza  )){
                  paux = paux-> sgte;
                }
                if(paux){
                    aux = paux->sublista;
                    if(aux){
                       while( aux && aux->sgte && strcmp(aux->Incidente.fechaHora, Incidente.fechaHora) !=0) {
                           aux = aux->sgte;
                       }
                       if(aux->sgte == NULL){
                           if(strcmp(aux->Incidente.fechaHora, Incidente.fechaHora) !=0){
                             paux->Poliza.cant_incidentes ++;
                            AgregarElemento2(aux->sgte,Incidente);  
                           } 
                       }
                    } else {
                        paux->Poliza.cant_incidentes ++;
                        AgregarElemento2(paux->sublista,Incidente);
                    }
                  
                } else {
                  cout << "No se encontró la poliza: " << Incidente.nro_poliza << endl;
                }
            }
          fclose(f);
        }
    } else {
        cout << "Lista vaica !!" << endl;
    }
    
return;
}

int contarAccidentes(Nodo *lista) //Cuenta los accidentes que tuvo ese nro de poliza en el año 2020
{
    Nodo *aux=lista;
    FILE *f;
    registroIncidente buffer;
    int cont=0;
    f=fopen(ARCHIVO_PROCESAR,"rb");
    while(fread(&buffer,sizeof(registroIncidente),1,f))
    {
        if(buffer.nro_poliza==aux->Poliza.nro_poliza && (atoi(buffer.fechaHora)>=2020000000 && atoi(buffer.fechaHora)<2021000000))
        {
            cont++;
        }
    }
    fclose(f);
    return cont;
}

void mostrar_polizaHTML(Nodo *lista_RegistroAsegurado)
{
    FILE *incidentes;
    FILE *f;
    char estado[50];
    registroIncidente ri;
    f = fopen("index.html", "wt");
    fprintf(f,"<html><body>\n");
    fprintf(f,"<h1>Polizas con mas de 5 incidentes durante el 2020</h1>\n");
    fprintf(f,"<table border=5>\n");
    fprintf(f,"<th>Nro de poliza</th><th>DNI</th><th>Apellido</th><th>Nombre</th><th>Patente</th><th>Estado</th><th>Cantidad de Incidentes</th>\n");
    while(lista_RegistroAsegurado)
    {
        if (lista_RegistroAsegurado->Poliza.cant_incidentes >5)
        {
            if (incidentes=fopen(ARCHIVO_PROCESAR,"rb"))
            {

                while(fread(&ri, sizeof(registroIncidente),1,incidentes))
                {

                    if(lista_RegistroAsegurado->Poliza.nro_poliza == ri.nro_poliza && atoi(ri.fechaHora)-2020000000<= 123123)//durante el 2020
                    {
                        if (lista_RegistroAsegurado->Poliza.cuota)
                        {
                            strcpy(estado,"Al dia");
                        }
                        else
                        {
                            strcpy(estado,"Presenta deudas");
                        }
                        fprintf(f,"<tr>\n");
                        fprintf(f,"<td>%d</td><td>%s</td><td>%s</td><td>%s</td><td>%s</td><td>%s</td><td>%d</td>\n",
                                lista_RegistroAsegurado->Poliza.nro_poliza,
                                lista_RegistroAsegurado->Poliza.dniAsegurado,
                                lista_RegistroAsegurado->Poliza.apellido,
                                lista_RegistroAsegurado->Poliza.nombre,
                                lista_RegistroAsegurado->Poliza.patente,
                                estado,
                                lista_RegistroAsegurado->Poliza.cant_incidentes);
                        fprintf(f,"</tr>\n");
                    }

                }
                fseek(incidentes,0,SEEK_SET);
                fclose(incidentes);
            }
            else
            {
                cout << "Error de archivo" << endl;
            }
        }
        lista_RegistroAsegurado = lista_RegistroAsegurado->sgte;
    }
    fprintf(f, "</table>");
    fprintf(f, "</body>");
    fprintf(f, "</html>");
    fclose(f);
    return;
}

void mostrar_polizaCSV(Nodo *lista)  
{
    Nodo *aux=lista;
    FILE *f;
    f=fopen("Salida Excel.CSV","wt");
    int accidentes;
    char estado[7];
    if(f)
    {
        fprintf(f,"Poliza Nro;DNI;Nombre;Apellido;Estado de Cuota;Patente;Cantidad de Incidentes\n");
        do
        {
            accidentes=contarAccidentes(aux);
            if (accidentes>5)
            {
                if (aux->Poliza.cuota)
                {
                    strcpy(estado,"Al dia");
                }
                else
                {
                    strcpy(estado,"Adeuda");
                }
                fprintf(f,"%d;%s;%s;%s;%s;%s;%d",aux->Poliza.nro_poliza,aux->Poliza.dniAsegurado,aux->Poliza.nombre,aux->Poliza.apellido,estado,aux->Poliza.patente,aux->Poliza.cant_incidentes);
            }
            aux=aux->sgte;
        } while (aux);
    }
    else
    {
        cout << "Error de archivo" << endl;
    }
    fclose(f);
    return;
}

void mostrarMenu()
{
	cout << "1 - Cargar nueva poliza" << endl;
	cout << "2 - Desactivar poliza existente" << endl;
	cout << "3 - Buscar una póliza" << endl;
	cout << "4 - Listar polizas activas" << endl;
	cout << "5 - Procesar lote de incidentes" << endl;
	cout << "6 - Mostrar polizas con mas de 5 incidentes durante el 2020 en formato HTML" << endl;
	cout << "7 - Preparar reporte en formato CSV" << endl;
  cout << "ESC - Finalizar jornada" << endl;
 
}

int main(){
  Nodo *lista=NULL;
  levantar_cuentas(lista); 

  int npol;
  char opcion;

  do
  {
    mostrarMenu();  
    opcion = getch();

    switch(opcion)
    {
          case '1':
            cargar_pol(lista);   
          break;
    
          case '2':
            cout << " Ingrese el numero de poliza a desactivar: " << endl;
            cin >> npol;
            desactivar_poliza(npol,lista);  
          break;

          case '3':
            buscar_poliza(lista);  
          break;
          
          case '4':
          ordenar_polizas(lista);  
          listar_polizas(lista); 
          break; 

          case '5':
          cargar_indicentes();
          procesar_incidentes(lista); 
          break;
          
          case '6':
          mostrar_polizaHTML(lista); 
          break;    
          
          case '7':
          mostrar_polizaCSV(lista); 
          break;

    }

    cout << "\nPresione una tecla para continuar" << endl;
    getch();
    system("cls"); //para limpiar la pantalla
  } while( opcion!=27);

 finalizar_jornada(lista, lista->sublista); 

  //para no causar un memory leak
  limpiarListas(lista);
  return 0;
}
