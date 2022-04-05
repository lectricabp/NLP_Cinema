#include <iostream>
#include <vector>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <Windows.h>
#include <cstring>
#include <fstream>

using namespace std;

struct pelicula{
    vector<string> nom;
    string sala;
    string genere;
    string s1,s2,s3;
    string duracio;
    string nameID;
};
struct user_film{
    vector<string> nom;
    string genere;
    string s1;
};

void llegircartellera(vector<pelicula> &t);
vector<string> split(string t,char del);
void ensenyarCartellera(vector<pelicula> t);
vector<string> unique(vector<pelicula> t);
void ensenyarGeneres(vector<pelicula> t);
void ensenyarPelis(vector<pelicula> t);
vector<string> llegirMissatge();
void cases(int caseFilm, vector<pelicula> t, user_film &request, bool &finish);
void ensenyarTipusGeneres(vector<pelicula> t, string gen);
void entradaPeli(vector<pelicula> t, vector<string> name);
void entradaHora(vector<pelicula> t ,vector<string> name, string hora);
bool comprovarTitol(vector<string> title, vector<string> name);
string checkHour(vector<string> msg);
void buscarParaulesClau(vector<string> msg, vector<pelicula> t,int &caseFilm, user_film &request);

ofstream myfile ("conversation.txt");

int main(){
	
	system("color 0F");
	
	if(myfile.is_open()){
		bool finish=false;
		vector<string> input;
		int case_ans;
		vector<pelicula> cartellera;
		user_film request;
		llegircartellera(cartellera);
	
		cout<<"MISCO: Hola! Soc en MISCO! Soc el teu assistent al cinema! Que vols fer?\n";
		myfile<<"MISCO: Hola! Soc en MISCO! Soc el teu assistent al cinema! Que vols fer?\n";
		while (!finish) {
			input = llegirMissatge();
			buscarParaulesClau(input,cartellera,case_ans,request);
			cases(case_ans,cartellera,request,finish);
		    cout << "Presioni qualsevol tecla per continuar:";
		    cin.get();
		    system("CLS");
			if (case_ans!=7){
				cout<<"MISCO: Que mes vols fer?\n\n";
				myfile<<"MISCO: Que mes vols fer?\n";
				case_ans = 0;
			}
	
		}
		myfile.close();
	}
	else cout<<"\nMISCO: No he pogut iniciar-me correctament..."<<endl;
}

void buscarParaulesClau(vector<string> msg, vector<pelicula> t,int &caseFilm,user_film &request){

	bool show,quines,genere,pelisName,genereName,hora = false;
	int i = 0;
	while(i < msg.size()){
		if(msg[i] == "cartellera" || msg[i] == "Cartellera" ) caseFilm=1;
		else if((msg[i] == "mostra" || msg[i] == "Mostra" || msg[i] == "ensenyar" || msg[i] == "Ensenyar") && !show){
			show = true;
      		caseFilm=1;
			i = -1;
		}

		else if((msg[i] == "quines" || msg[i] == "Quines") && !quines){
			quines = true;
			i = -1;
		}
		else if((msg[i] == "pelicules" || msg[i] == "Pelicules" || msg[i] == "pel.licules" || msg[i] == "Pel.licula" )&& (show || quines)) caseFilm=2;

		else if((msg[i] == "genere" || msg[i] == "Genere" || msg[i] == "generes" || msg[i] == "Generes") && !genere){
			if(genereName){
				genere = true;
				i = -1;
			}
			if(genereName) caseFilm=4;
			else caseFilm=3;
		}
		else if(msg[i] == "seient" || msg[i] == "Seient" || msg[i] == "entrada" || msg[i] == "Entrada" || msg[i] == "entrades" || msg[i] == "Entrades" || msg[i] == "comprar" || msg[i] == "Comprar" ){
			if(pelisName && hora){
        		caseFilm=6;
      		}
			else if(pelisName){
				string hourName = checkHour(msg);
				if(hourName!="0"){
					request.s1=hourName;
					hora = true;
					caseFilm=6;

				}

				if(!hora) caseFilm=5;
			}
		}
		else if(msg[i]=="Sortir" || msg[i]=="sortir") caseFilm = 7;
		else{
			// check pelis
			if(!pelisName){
				int j = 0;
				while(j<t.size() && !pelisName){
					if(msg[i]==t[j].nameID){
						pelisName = true;
	          			request.nom = t[j].nom;
	          			i = -1;
					}
					j++;
				}
			}
			// check generes
			if(!genereName && !pelisName){
				vector<string> generesName = unique(t);
				int k = 0;
				while(k<generesName.size() && !genereName){
					if(msg[i]==generesName[k]){
						genereName = true;
            			request.genere=msg[i];
						i = -1;
					}
					k++;
				}
			}
		}
		i++;
	}
}

void llegircartellera(vector<pelicula> &t){
    ifstream f("input.txt");
    vector<string> temp2;
    string temp;
    string f_cont;
    pelicula c;
    while (getline(f,temp,'\n')){
		temp2 = split(temp,' ');
		c.genere = temp2[0];
		c.s1 = temp2[1];
		c.s2 = temp2[2];
		c.s3 = temp2[3];
		c.sala = temp2[4];
		c.duracio = temp2[5];
		c.nameID = temp2[6];

      for (size_t i = 7; i < temp2.size(); i++) {
        c.nom.push_back(temp2[i]);
      }

      t.push_back(c);
      c.nom.clear();

	  }
  }

vector<string> split(string t, char del){
  vector<string> x;
  int cont= 0;
  string word;

  for (size_t i = 0; i < t.size(); i++) {
	while (t[i] != del && i<t.size()) {
	  	word.push_back(t[i]);
	  	i++;
    }
    x.push_back(word);
    cont++;
    word.clear();

  }
  return x;
}

vector<string> llegirMissatge(){
   string temp;
   vector<string> msg;
   getline(cin,temp);
   msg = split(temp,' ');
   myfile<<"USUARI: "<<temp<<endl;
   return msg;
}

void ensenyarTipusGeneres(vector<pelicula> t, string gen){
  for (size_t i = 0; i < t.size(); i++){
    if (t[i].genere == gen){
      for (size_t j = 0; j < t[i].nom.size(); j++) cout << t[i].nom[j] << " ";
      cout << '\n';
    }
  }
}

void entradaPeli(vector<pelicula> t, vector<string> name){
  bool trobat = false;
  for (size_t i = 0; i < t.size(); i++){
    trobat = comprovarTitol(t[i].nom,name);
    if (trobat){
		cout<<"\nMISCO: No has introduit cap hora. Aquestes son les hores disponibles!\n";
		myfile<<"MISCO: No has introduit cap hora. Aquestes son les hores disponibles!\n";
        cout << t[i].s1 << " | "<<t[i].s2 <<" | "<<t[i].s3 << "\n";
        myfile << t[i].s1 << " | "<<t[i].s2 <<" | "<<t[i].s3 << "\n";
      }
    }
  }

bool comprovarTitol(vector<string> title, vector<string> name){
  bool test;
  int cont;
  for (size_t i = 0; i < title.size(); i++) {
    if (title[i] == name[i]) {
      cont++;
    } else{
      break;
    }
    if (cont == static_cast<int>(title.size()))
    {
       test = true;
       break;
    }
  }
  return test;
}

void entradaHora(vector<pelicula> t ,vector<string> name, string hora){
	bool trobat = false;
	bool trobathora = false;
  	bool trobatno = false;
	vector<string> decisio;
  	size_t i = 0;
	while (i<t.size() && !trobat) {
		trobat = comprovarTitol(t[i].nom,name);
    	if (trobat){
			if (hora == t[i].s1|| hora == t[i].s2|| hora == t[i].s3){
	         	trobathora = true;
	         	cout<<"MISCO: Perfecte! El seu seient esta situat a la fila "<< rand() % 20 + 1<<" columna "<<rand() % 30 + 1<<". Vol modificar-lo?"<<endl;
	         	myfile<<"MISCO: Perfecte! El seu seient esta situat a la fila "<< rand() % 20 + 1<<" columna "<<rand() % 30 + 1<<". Vol modificar-lo?"<<endl;
				decisio=llegirMissatge();
            for (size_t i = 0; i < decisio.size(); i++) {
              if(decisio[i] == "No" || decisio[i] == "no" || decisio[i] == "NO") trobatno = true;
            }
  	        while(!trobatno){
  	         		cout<<"MISCO: El nou seient assignat esta a la fila "<< rand() % 20 + 1<<" columna "<<rand() % 30 + 1<<". Vol modificar-lo?"<<endl;
					myfile<<"MISCO: El nou seient assignat esta a la fila "<< rand() % 20 + 1<<" columna "<<rand() % 30 + 1<<". Vol modificar-lo?"<<endl;
  	         		decisio=llegirMissatge();
                for (size_t i = 0; i < decisio.size(); i++) {
                  if(decisio[i] == "No" || decisio[i] == "no" || decisio[i] == "NO") trobatno = true;
                }
  	        }

            cout << "\nMISCO: Gaudeixi de la pelicula!\n";
            myfile << "MISCO: Gaudeixi de la pelicula!\n";
         	}
      	}
        i++;
    }

    if(!trobat){
    	cout<<"MISCO: No oferim aquesta pelicula a la cartellera. Ho sentim"<<endl;
    	myfile<<"MISCO: No oferim aquesta pelicula a la cartellera. Ho sentim"<<endl;
    }
    else{
    	if(!trobathora){
	    	cout<<"MISCO: No oferim aquesta pelicula en aquest horari. Ho sentim"<<endl;
	    	myfile<<"MISCO: No oferim aquesta pelicula en aquest horari. Ho sentim"<<endl;
	    }
    }
}

void cases(int caseFilm, vector<pelicula> t, user_film &request, bool &finish){
	string hourName = "0";
	vector<string> msg;
	switch(caseFilm){
		case 1:
			cout<<"\nMISCO: Ara mateix! Aqui la tens!\n\n";
			myfile<<"MISCO: Ara mateix! Aqui la tens!\n\n";
			ensenyarCartellera(t);
		break;
		case 2:
			cout<<"\nMISCO: Ara mateix! Aqui te les mostro!\n\n";
			myfile<<"MISCO: Ara mateix! Aqui te les mostro!\n\n";
			ensenyarPelis(t);
		break;
		case 3:
			cout<<"\nMISCO: D'acord, et mostro les pel.licules agrupades per generes!\n\n";
			myfile<<"\nMISCO: D'acord, et mostro les pel.licules agrupades per generes!\n\n";
			ensenyarGeneres(t);
		break;
		case 4:
			cout<<"\nMISCO: Genial! Et mostro les pel.licules de "<<request.genere<<"!\n\n";
			myfile<<"\nMISCO: Genial! Et mostro les pel.licules de "<<request.genere<<"!\n\n";
			ensenyarTipusGeneres(t,request.genere);
		break;
		case 5:
			entradaPeli( t, request.nom);
			cout<<"\nMISCO: Quina sessio vols?\n\n";
			myfile<<"MISCO: Quina sessio vols?\n\n";
			msg = llegirMissatge();
			hourName = checkHour(msg);
			while(hourName == "0"){
				cout<<"\nMISCO: Error. Si-us-plau, introdueix la hora amb el format hh:mm"<<endl;
				myfile<<"\nMISCO: Error. Si-us-plau, introdueix la hora amb el format hh:mm"<<endl;
				msg = llegirMissatge();
				hourName = checkHour(msg);
			}
			request.s1 = hourName;
			entradaHora(t ,request.nom, request.s1);
		break;
		case 6:
			entradaHora(t ,request.nom, request.s1);
		break;
		case 7:
			cout<<"MISCO: Fins la propera!\n"<<endl;
			myfile<<"MISCO: Fins la propera!"<<endl;
			finish = true;
		break;
		default:
			cout<<"MISCO: Perdona, no t'he entes. Quina era la teva pregunta?\n"<<endl;
			myfile<<"MISCO: Perdona, no t'he entes. Quina era la teva pregunta?"<<endl;
	}
}

void ensenyarCartellera(vector<pelicula> t){
    for (size_t i = 0; i < t.size(); i++) {
		cout<<"======== ";
		myfile<<"======== ";
		for (size_t j = 0; j < t[i].nom.size(); j++){
			cout << t[i].nom[j] << " ";
			myfile << t[i].nom[j] << " ";
		}
		cout<<"========";
		myfile<<"======== ";
		cout <<"\nGenere: "<<t[i].genere << "\n";
		myfile <<"\nGenere: "<<t[i].genere << "\n";
		cout << "Sesions: "<<t[i].s1 << " "<<t[i].s2 <<"  "<<t[i].s3 << "\n\n";
		myfile << "Sesions: "<<t[i].s1 << " "<<t[i].s2 <<"  "<<t[i].s3 << "\n\n";
    }
}

void ensenyarPelis(vector<pelicula> t){
	cout<<"Pel.licules disponibles a cartellera\n";
	cout<<"------------------------------------\n\n";
	myfile<<"Pel.licules disponibles a cartellera\n";
	myfile<<"------------------------------------\n\n";
    for (size_t i = 0; i < t.size(); i++) {
		for (size_t j = 0; j < t[i].nom.size(); j++) {
			cout << t[i].nom[j] << " ";
			myfile << t[i].nom[j] << " ";
		}
		cout<<"\n";
		myfile<<"\n";
    }
}

void ensenyarGeneres(vector<pelicula> t){
	vector<string> generes = unique(t);
	cout<<"Pel.licules per genere\n";
	cout<<"----------------------\n";
	myfile<<"Pel.licules per genere\n";
	myfile<<"----------------------\n";
	for(int i = 0; i<generes.size();i++){
		cout<<"\n=========="<<generes[i]<<"==========\n";
		myfile<<"\n=========="<<generes[i]<<"==========\n";
		for(int j = 0;j<t.size();j++){
			if(t[j].genere == generes[i]){
				for (size_t k = 0; k < t[j].nom.size(); k++){
					cout << t[j].nom[k] << " ";
					myfile << t[j].nom[k] << " ";
				}
				cout<<"\n";
				myfile<<"\n";
			}
		}
	}
}

vector<string> unique(vector<pelicula> t){
	vector<string> unq;
	int j = 0;
	bool genFound = false;
	for(int i=0; i<t.size();i++){
		j = 0;
		genFound = false;
		while(j<unq.size()){
			if(t[i].genere==unq[j]){
				genFound = true;
				break;
			}
			j++;
		}
		if(!genFound) unq.push_back(t[i].genere);
	}
	return unq;
}

string checkHour(vector<string> msg){
	for(int i=0;i<msg.size();i++){
		string word = msg[i];
		int n = word.length();
 		if(n==5){

		    char char_array[n + 1];

		    strcpy(char_array, word.c_str());

			for(int j=0;j<n;j++){
				if(char_array[j] == ':' ){
					if(j==2) return word;
				}
			}
		}

	}
	return "0";
}
