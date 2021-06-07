#include "Util.h"
#include <assert.h>
void UTIL::transform(vector<string> *t, string *destino)
{
  itv=(*t).begin();
  (*destino).append(*itv);
  itv++;
  for(;itv!=(*t).end();++itv)
  {
    (*destino).append(vacio);
    (*destino).append(*itv);
  }
}

//--------------------------------------------

void UTIL::preparar(string query, string *entrada)
{
  tokens.clear();
  terminos.clear();

  this->Tokenize(query,tokens," ");
  if(tokens.size()<1)
  {
    cout << "ERROR: consulta mala en preparar=" << query << endl;
    exit(0);
  }

  //eliminar las iteraciones
  //int i=0;
  for(itv=tokens.begin();
      itv!=tokens.end();
      ++itv)
  {
    //if(i%2==0) terminos.push_back(*iter);
    //i++;
    terminos.push_back(*itv);
  }

  //ordenar los terminos (CON REPETICIONES)
  sort(terminos.begin(),terminos.end());

  //pasarlos a un string de terminos ordenados
  (*entrada).clear();
  this->transform(&terminos,entrada);
}

//--------------------------------------------

double UTIL::getCost(string query, map<int,int> *words)
{
  suma=0.0;
  tokens.clear();

  this->Tokenize(query,tokens," ");
  for(itv=tokens.begin();
      itv!=tokens.end();
      ++itv)
  {
    if((*words).count(atoi((*itv).c_str()))!=0)
    suma=suma+(*words)[atoi((*itv).c_str())];
  }

   if(suma==0.0)
   suma == -1.0;
//  ASSERT(suma>=0.0);
  //return suma;
  return log(suma);
}

//--------------------------------------------
void UTIL::Tokenize(const string& str,
                    vector<string>& tokens,
                    const string& delimiters = " ")
{
  string::size_type lastPos = str.find_first_not_of(delimiters, 0);
  string::size_type pos     = str.find_first_of(delimiters, lastPos);
  while (string::npos != pos || string::npos != lastPos)
  {
    tokens.push_back(str.substr(lastPos, pos - lastPos));
    lastPos = str.find_first_not_of(delimiters, pos);
    pos = str.find_first_of(delimiters, lastPos);
  }
}

//--------------------------------------------
/*
void UTIL::leer_words(char *_file, map<int,int> *words)
{
  ifstream fin(_file);
  ASSERT(fin.is_open());

  string linea;
  int i,idt,largo;

  i=0;
  while(getline(fin,linea))
  {
    tokens.clear();
    this->Tokenize(linea,tokens,",");
    if(tokens.size()!=3)
    {
      cout << "ERROR: word mala=" << linea << endl;
      exit(0);
    }

    idt=atoi(tokens.at(1).c_str());
    largo=atoi(tokens.at(2).c_str());
    ASSERT((*words).count(idt)==0);
    (*words)[idt]=largo;

    i++;
  }
  fin.close();
  cout << "i=" << i << endl;
  cout << "words.size()=" << (*words).size() << endl;
}

//--------------------------------------------

void UTIL::leer_mapeo(char *_file, map<int,int> *mapeo)
{
  ifstream fin(_file);
  ASSERT(fin.is_open());

  string linea;
  int i,iddoc,proc;

  i=0;
  while(getline(fin,linea))
  {
    //formato: iddoc maquina
    tokens.clear();
    this->Tokenize(linea,tokens," ");
    if(tokens.size()!=2)
    {
      cout << "ERROR: mapeo malo=" << linea << endl;
      exit(0);
    }

    proc=atoi(tokens.at(0).c_str());
    iddoc=atoi(tokens.at(1).c_str());
    if((*mapeo).count(iddoc)==0)
//  ASSERT((*mapeo).count(iddoc)==0);
   { (*mapeo)[iddoc]=proc;

    i++;}
  }
  fin.close();
  cout << "i=" << i << endl;
  cout << "mapeo.size()=" << (*mapeo).size() << endl;
}
*/
int UTIL::getProcs(string docs, map<int,int> *mapeo)
{
  cout << docs << endl;
  tokens.clear();
  procs.clear();

  this->Tokenize(docs,tokens," ");
  if(tokens.size()>=1)
  for(itv=tokens.begin();itv!=tokens.end();++itv)
  {
    //cout << *itv << ":" << strtoul((*itv).c_str(),NULL,0) << endl;
    cout << *itv << ":" << atoi((*itv).c_str()) << endl;
    if((*mapeo).count( atoi((*itv).c_str()) )!=0)
      proc=(*mapeo)[ atoi((*itv).c_str()) ];
    else
     proc=atoi((*itv).c_str()) % P;
    procs.insert( proc );
  }
  cout << "-------------------" << endl;
  return procs.size();
}

