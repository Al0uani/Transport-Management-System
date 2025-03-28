#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <iomanip>
#include <cstring>
#include <direct.h>
#include <thread>
#include <memory.h>
#include <filesystem>
#include <cstdlib>
#include <unistd.h>
#include <ctype.h>
using namespace std;

int NumModif=0;
const char* username=getenv("USERNAME");
class Marchandise {
    private:
        int numero;
        double poids, volume;
    public:
        static int IncNum;
        Marchandise(double poids, double volume,int numero) : poids(poids), volume(volume), numero(numero) {}
        Marchandise(double poids, double volume) : poids(poids), volume(volume), numero(IncNum++) {}
        int getNumero() { return numero; }
        double getPoids() { return poids; }
        double getVolume() { return volume; }
        void setPoids(double new_poids) { poids = new_poids; }
        void setVolume(double new_volume) { this->volume = new_volume; }
        void afficher(){cout<<"\nNumero:"<<numero<<"\tPoids:"<<poids<<"\tvolume:"<<volume<<endl;}
};
int Marchandise::IncNum = 1;

class Cargaison {
    private:
        string Reference;
        double Distance;
        unordered_map<int, Marchandise> Marchandises;

    public:
        Cargaison(string Reference, double Distance) : Reference(Reference), Distance(Distance) {}

        virtual double cout_transport() = 0;

        string getRef() { return Reference; }

        double getDistance() { return Distance; }

        unordered_map<int, Marchandise> getMarchandises() { return Marchandises; }
        void setMarchandises(Marchandise march){int n=march.getNumero(); Marchandises.insert(make_pair(n,march));}
        void clearenceMarchandise(){Marchandises.clear();}

        vector<double> Total_Poids_Volume() {
            vector<double> results;
            double accumPoids = 0;
            double accumVolume = 0;
            for (auto &Pair : Marchandises) {
                Marchandise &element = Pair.second;
                accumPoids += element.getPoids();
                accumVolume += element.getVolume();}
            results.push_back(accumPoids);
            results.push_back(accumVolume);
            return results;
        }
        virtual void afficher()=0;


};

class CargaisonRoutiere : public Cargaison {
public:
    CargaisonRoutiere():Cargaison("",0){}
    CargaisonRoutiere(string Reference,double Distance):Cargaison(Reference,Distance){}
    double cout_transport() override {
        double charge;vector<double>Totalite=Total_Poids_Volume();
        if(Totalite[1]<380000){charge=4*getDistance()*Totalite[0];}
        else if(Totalite[1]>=380000){charge=6*getDistance()*Totalite[0];}
        return charge;
    }
    void afficher()override{
        cout<<"\nCargaisonRoutiere:"<<endl;
        cout<<"\nRef:"<<getRef()<<"\tDistance:"<<getDistance()<<endl;
        cout<<"Les Marchandises:";auto TempMarch=getMarchandises();
        if(TempMarch.empty()){cout<<"Vide"<<endl;}
        else{for(auto &Pair:TempMarch){Marchandise &element=Pair.second;element.afficher();}printf("Cout:%.2f\n",cout_transport());}
    }
};

class CargaisonAerienne:public Cargaison{
    public:
    CargaisonAerienne():Cargaison("",0){}
    CargaisonAerienne(string Reference,double Distance):Cargaison(Reference,Distance){}
    double cout_transport() override {
        double charge;vector<double>Totalite=Total_Poids_Volume();
        if(Totalite[1]<80000){charge=10*getDistance()*Totalite[0];}
        else if(Totalite[1]>=80000){charge=12*getDistance()*Totalite[0];}
        return charge;
    }
     void afficher()override{
        cout<<"CargainsonArienne:"<<endl;
        cout<<"\nRef:"<<getRef()<<"\tDistance:"<<getDistance()<<endl;
        cout<<"Les Marchandises:";auto TempMarch=getMarchandises();
        if(TempMarch.empty()){cout<<"Vide"<<endl;}
        else{for(auto &Pair:TempMarch){Marchandise &element=Pair.second;element.afficher();}printf("Cout:%.2f\n",cout_transport());}
    }
};

class ClientTransport{ // Valid_Testing_1
    public:
        void Consulter_Cargaison_Ref(unordered_map<string,CargaisonRoutiere> CR,string Ref){ // Valid_Testing_1
            if(Ref[0]=='C'&& Ref[1]=='R'){
                auto it=CR.find(Ref);
                if(it!=CR.end()){CargaisonRoutiere &element=it->second;element.afficher();}
                else{cout<<"\n\t Reference introuvable dans Les cargaison Routiere"<<endl;}}
            else{cout<<"\nReference Invalid"<<endl;}}

     void Consulter_Cargaison_Ref(unordered_map<string,CargaisonAerienne> CA,string Ref){ // Valid_Testing_1
            if(Ref[0]=='C'&& Ref[1]=='A'){
                auto it=CA.find(Ref);
                if(it!=CA.end()){CargaisonAerienne &element=it->second;element.afficher();}
                else{cout<<"\n\t Reference introuvable dans Les cargaison Aerienne"<<endl;}}
            else{cout<<"\nReference Invalid"<<endl;}}

        void Consulter_Marchandise_Num(unordered_map<string,CargaisonAerienne>CA,int num){ // Valid_Testing_1
            bool flag=false;
                for(auto &it:CA){CargaisonAerienne &element= it.second;auto march=element.getMarchandises();
                    auto i=march.find(num);
                    if(i!=march.end()){cout<<"Cargaison Aerienne"<<endl;Marchandise &tmp=i->second;tmp.afficher();flag=true;break;}}
            if(flag==false){cout<<"\nMarchandise introuvable dans Les cargaison Aerienne"<<endl;}
        }

        void Consulter_Marchandise_Num(unordered_map<string,CargaisonRoutiere> CR,int num){ // Valid_Testing_1
            bool flag=false;
                for(auto &it:CR){CargaisonRoutiere &element= it.second;auto march=element.getMarchandises();
                    auto i=march.find(num);
                    if(i!=march.end()){cout<<"Cargaison Routiere"<<endl;Marchandise &tmp=i->second;tmp.afficher();flag=true;break;}}
            if(!flag){cout<<"\nMarchandise introuvable dans Les cargaison Routiere"<<endl;}
        }

        void Consulter_marchandises(auto &Cargo){Cargo.afficher();} // Valid_Testing_1

        void Consulter_cargaisons(unordered_map<string, CargaisonAerienne> CA) { // Valid_Testing_1
            for (auto &entry : CA) {entry.second.afficher();}
        }

         void Consulter_cargaisons(unordered_map<string,CargaisonRoutiere> CR){ // Valid_Testing_1
            for(auto &Pair:CR){Pair.second.afficher();}
        }

        void Consulter_poids_total_cargaison(auto Cargo){auto x=Cargo.Total_Poids_Volume();cout<<"\nPoids Total:"<<x[0]<<endl;} // Valid_Testing_1

        void Consulter_cout_transport_cargaison(auto Cargo){printf("\nCout:%.2f\n",Cargo.cout_transport());} // Valid_Testing_1

        void Trier_cargaisons(unordered_map<string,CargaisonRoutiere>& Cargo){ // Valid_Testing_1
            vector<string>Ref;vector<double>Cout;double temp; string Reftemp;unordered_map<string,CargaisonRoutiere> results;
            for(auto &Pair:Cargo){CargaisonRoutiere &element=Pair.second;Ref.push_back(Pair.first);Cout.push_back(element.cout_transport());}
            for(int i=0;i<Ref.size();i++){
                for(int j=0;j<Ref.size();j++){
                    if(Cout[j]<Cout[i]){temp=Cout[j];Cout[j]=Cout[i];Cout[i]=temp;Reftemp=Ref[j];Ref[j]=Ref[i];Ref[i]=Reftemp;}
                }}
            for(int i=0;i<Ref.size();i++){
                auto it=Cargo.find(Ref[i]);
                if(it!=Cargo.end()){results.insert(make_pair(Ref[i],it->second));}
            }
            Cargo.clear(); Cargo=results;Ref.clear();Cout.clear();results.clear();
        }

        void Trier_cargaisons(unordered_map<string,CargaisonAerienne> &Cargo){ // Valid_Testing_1
            vector<string>Ref;vector<double>Cout;double temp; string Reftemp;unordered_map<string,CargaisonAerienne> results;
            for(auto &Pair:Cargo){CargaisonAerienne &element=Pair.second;Ref.push_back(Pair.first);Cout.push_back(element.cout_transport());}
            for(int i=0;i<Ref.size();i++){
                for(int j=0;j<Ref.size();j++){
                    if(Cout[j]<Cout[i]){temp=Cout[j];Cout[j]=Cout[i];Cout[i]=temp;Reftemp=Ref[j];Ref[j]=Ref[i];Ref[i]=Reftemp;}
                }}
            for(int i=0;i<Ref.size();i++){
                auto it=Cargo.find(Ref[i]);
                if(it!=Cargo.end()){results.insert(make_pair(Ref[i],it->second));}
            }
            Cargo.clear(); Cargo=results;Ref.clear();Cout.clear();results.clear();

        }

};


class AdminTransport:public ClientTransport{
    public:
        void Ajoute_cargaison(unordered_map<string,CargaisonAerienne> &les_cargo,CargaisonAerienne Cargo){ // Valid_Testing_1
            string Refe=Cargo.getRef();
            les_cargo.insert(make_pair(Refe,Cargo));NumModif+=1;
            }
        void Ajoute_cargaison(unordered_map<string,CargaisonRoutiere> &les_cargo,CargaisonRoutiere Cargo){ // Valid_Testing_1
            string Refe=Cargo.getRef();
            les_cargo.insert(make_pair(Refe,Cargo));NumModif+=1;}

        void Ajoute_Marchandise(unordered_map<string,CargaisonAerienne> &les_cargo,string REF,Marchandise march){ // Valid_Testing_1
            auto it = les_cargo.find(REF);
            if(it!=les_cargo.end()){
                CargaisonAerienne &elemnt=it->second;
                elemnt.setMarchandises(march);NumModif+=1;cout<<"\n\tMarchandise Ajoutee"<<endl;}
            else{cout<<"\nReference introuvable"<<endl;}
        }
        void Ajoute_Marchandise(unordered_map<string,CargaisonRoutiere> &les_cargo,string REF,Marchandise march){ // Valid_Testing_1
            auto it = les_cargo.find(REF);
            if(it!=les_cargo.end()){
                CargaisonRoutiere &elemnt=it->second;
                elemnt.setMarchandises(march);NumModif+=1;cout<<"\n\tMarchandise Ajoutee"<<endl;}
            else{cout<<"\nReference introuvable"<<endl;}
        }
        void suppression_marchandise(unordered_map<string,CargaisonRoutiere> &les_cargo,string REF,int num){ // Valid_Testing_1
            auto it=les_cargo.find(REF);
            if(it!=les_cargo.end()){
                CargaisonRoutiere &elemnt=it->second;
                auto Marchan = elemnt.getMarchandises();auto x=Marchan.find(num);
                if(x!=Marchan.end()){
                    Marchan.erase(x);elemnt.clearenceMarchandise();
                    for(auto &i:Marchan){elemnt.setMarchandises(i.second);}NumModif+=1;cout<<"\n\tMarchandise Supprimee"<<endl;
                }else{cout<<"\nNumero introuvable"<<endl;}
            }else{cout<<"\nReference introuvable"<<endl;}
        }
        void suppression_marchandise(unordered_map<string,CargaisonAerienne> &les_cargo,string REF,int num){ // Valid_Testing_1
            auto it=les_cargo.find(REF);
            if(it!=les_cargo.end()){
                CargaisonAerienne &elemnt=it->second;
                auto Marchan = elemnt.getMarchandises();auto x=Marchan.find(num);
                if(x!=Marchan.end()){
                    Marchan.erase(x);elemnt.clearenceMarchandise();
                    for(auto &i:Marchan){elemnt.setMarchandises(i.second);}NumModif+=1;cout<<"\n\tMarchandise Supprimee"<<endl;
                }else{cout<<"\nNumero introuvable"<<endl;}
            }else{cout<<"\nReference introuvable"<<endl;}
        }
        void suppression_Cargo(unordered_map<string,CargaisonAerienne> &les_cargo,string REF){
            auto it=les_cargo.find(REF);
            if(it!=les_cargo.end()){les_cargo.erase(it);NumModif+=1;cout<<"\n\tCargaison Supprimee"<<endl;}else{cout<<"\nReference introuvable"<<endl;}
        }
        void suppression_Cargo(unordered_map<string,CargaisonRoutiere> &les_cargo,string REF){
            auto it=les_cargo.find(REF);
            if(it!=les_cargo.end()){les_cargo.erase(it);NumModif+=1;cout<<"\n\tCargaison Supprimee"<<endl;}else{cout<<"\nReference introuvable"<<endl;}
        }

    void modifier_marchandise(unordered_map<string, CargaisonAerienne> &les_cargo, string REF, int num) { // Valid_Testing_1
        auto it=les_cargo.find(REF);
        if(it!=les_cargo.end()){
            CargaisonAerienne &elmnt=it->second; auto MarchSearch=elmnt.getMarchandises();
            auto i=MarchSearch.find(num);
            if(i!=MarchSearch.end()){
                Marchandise march=i->second;
                 march.afficher();int choice;
                        loopVerify:
                        system("CLS");
                        cout<<"\n\t[1]-Modifier le Poids\n\t[2]-Modifier le volume\n\tChoice:";cin>>choice;
                        switch(choice){
                            case 1:{double New_Poids;cout<<"\nNouveau Poids:";cin>>New_Poids;
                                    if(New_Poids>0){
                                        suppression_marchandise(les_cargo,REF,num);
                                        Marchandise nouv(New_Poids,march.getVolume(),march.getNumero());
                                        Ajoute_Marchandise(les_cargo,REF,nouv);NumModif+=1;cout<<"\n\tMarchandise Modifiee"<<endl;
                                    }else{cout<<"\n\tInvalid Poids"<<endl;goto loopVerify;}}break;

                            case 2:{double New_Volume;cout<<"\nNouveau Volume:";cin>>New_Volume;
                                    if(New_Volume>0){
                                        suppression_marchandise(les_cargo,REF,num);
                                        Marchandise nouv(march.getPoids(),New_Volume,march.getNumero());
                                        Ajoute_Marchandise(les_cargo,REF,nouv);NumModif+=1;cout<<"\n\tMarchandise Modifiee"<<endl;
                                    }else{cout<<"\n\tInvalid Volume"<<endl;goto loopVerify;}}break;
                            default:
                                    cout<<"\n\tInvalid Input"<<endl;}

            }else{cout<<"\Marchandise introuvable"<<endl;}

        }else{cout<<"\nReference introuvable"<<endl;}
    }
    void modifier_marchandise(unordered_map<string, CargaisonRoutiere> &les_cargo, string REF, int num) { // Valid_Testing_1
        auto it=les_cargo.find(REF);
        if(it!=les_cargo.end()){
            CargaisonRoutiere &elmnt=it->second; auto MarchSearch=elmnt.getMarchandises();
            auto i=MarchSearch.find(num);
            if(i!=MarchSearch.end()){
                Marchandise march=i->second;
                 march.afficher();int choice;
                        cout<<"\n\t[1]-Modifier le Poids\n\t[2]-Modifier le volume\n\tChoice:";cin>>choice;
                        loopVerify:
                        switch(choice){
                            case 1:{double New_Poids;cout<<"\nNouveau Poids:";cin>>New_Poids;
                                    if(New_Poids>0){
                                        suppression_marchandise(les_cargo,REF,num);
                                        Marchandise nouv(New_Poids,march.getVolume(),march.getNumero());
                                        Ajoute_Marchandise(les_cargo,REF,nouv);NumModif+=1;
                                    }else{cout<<"\n\tInvalid Poids"<<endl;goto loopVerify;}}break;

                            case 2:{double New_Volume;cout<<"\nNouveau Volume:";cin>>New_Volume;
                                    if(New_Volume>0){
                                        suppression_marchandise(les_cargo,REF,num);
                                        Marchandise nouv(march.getPoids(),New_Volume,march.getNumero());
                                        Ajoute_Marchandise(les_cargo,REF,nouv);NumModif+=1;
                                    }else{cout<<"\n\tInvalid Volume"<<endl;goto loopVerify;}}break;
                            default:
                                    cout<<"\n\tInvalid Input"<<endl;}

            }else{cout<<"\Marchandise introuvable"<<endl;}

        }else{cout<<"\nReference introuvable"<<endl;}
    }

};
void FetchRef(unordered_set<string>&Refset){
    string path="C:\\Users\\"+string(username)+"\\MyRepo\\MiniProjet";
    _chdir(path.c_str());
    ifstream file;file.open("References.txt");string line;
    while(getline(file,line)){Refset.insert(line);}
    file.close();

}
void FileSave(auto Cargo){
    string path="C:\\Users\\"+string(username)+"\\MyRepo\\MiniProjet";
    _chdir(path.c_str());
    fstream file; file.open("References.txt",ios::app);{file<<Cargo.getRef()<<endl;}file.close();
    file.open("staticInit.txt",ios::out);{file<<Marchandise::IncNum<<endl;}file.close();
    string path1="C:\\Users\\"+string(username)+"\\MyRepo\\MiniProjet\\cargaisons";
    _chdir(path1.c_str());
    string Name=Cargo.getRef();Name+=".txt";

    file.open(Name,ios::app);
    if(file.is_open()){
        file<<Cargo.getDistance()<<endl;
        auto march=Cargo.getMarchandises();
        for(auto &pair:march){
            Marchandise m=pair.second;
            file<<m.getNumero()<<","<<m.getPoids()<<","<<m.getVolume()<<endl;
        }
    }
    file.close();

}
CargaisonAerienne loadFileA(string REF){
    string path="C:\\Users\\"+string(username)+"\\MyRepo\\MiniProjet\\cargaisons";
    _chdir(path.c_str());
    ifstream file;string nameFile=REF+".txt";int counter=0;string line;double dis;vector<Marchandise>Les_march;double poid,vol;int num;
    file.open(nameFile);
    while(getline(file,line)){
        int c=0;
        if(counter==0){dis=stod(line);counter++;}
        else{
            char*token=strtok(const_cast<char*>(line.c_str()),",");
            while(token!=nullptr){
                switch(c){
                    case 0:{num=stoi(token);c++;}break;
                    case 1:{poid=stod(token);c++;}break;
                    case 2:{vol=stod(token);c++;}break;}

                token=strtok(nullptr,",");
            }
            Marchandise m(poid,vol,num);Les_march.push_back(m);
        }
    }
    file.close();
        CargaisonAerienne ca(REF,dis);
        for(auto &x:Les_march){ca.setMarchandises(x);}Les_march.clear();
        return ca;

}
CargaisonRoutiere loadFileR(string REF){
    string path="C:\\Users\\"+string(username)+"\\MyRepo\\MiniProjet\\cargaisons";
    _chdir(path.c_str());
    ifstream file;string nameFile=REF+".txt";int counter=0;string line;double dis;vector<Marchandise>Les_march;double poid,vol;int num;
    file.open(nameFile);
    while(getline(file,line)){
        int c=0;
        if(counter==0){dis=stod(line);counter++;}
        else{
            char*token=strtok(const_cast<char*>(line.c_str()),",");
            while(token!=nullptr){
                switch(c){
                    case 0:{num=stoi(token);c++;}break;
                    case 1:{poid=stod(token);c++;}break;
                    case 2:{vol=stod(token);c++;}break;}

                token=strtok(nullptr,",");
            }
            Marchandise m(poid,vol,num);Les_march.push_back(m);
        }
    }
    file.close();
        CargaisonRoutiere cr(REF,dis);
        for(auto &x:Les_march){cr.setMarchandises(x);}Les_march.clear();
        return cr;

}
void FullLoad(unordered_map<string,CargaisonAerienne>&CA,vector<string>&CaRefs){
    for(int i=0;i<CaRefs.size();i++){
            CargaisonAerienne store=loadFileA(CaRefs[i]);
            CA.insert(make_pair(CaRefs[i],store));}
}
void FullLoad(unordered_map<string,CargaisonRoutiere>&CR,vector<string>&CrRefs){
    for(int i=0;i<CrRefs.size();i++){
            CargaisonRoutiere store=loadFileR(CrRefs[i]);
            CR.insert(make_pair(CrRefs[i],store));}
}
void StartUp_Protocol(unordered_set<string>&Refs,vector<string>&CaRefs,vector<string>&CrRefs,unordered_map<string,CargaisonAerienne>&CA,unordered_map<string,CargaisonRoutiere>&CR){ //Valid
        FetchRef(Refs);
        for(auto it=Refs.begin();it!=Refs.end();it++){
            string store=*it;
            if(store[1]=='A'){CaRefs.push_back(store);}
            else if(store[1]=='R'){CrRefs.push_back(store);}}
            FullLoad(CA,CaRefs);FullLoad(CR,CrRefs);
    string path="C:\\Users\\"+string(username)+"\\MyRepo\\MiniProjet";
    _chdir(path.c_str()); fstream file;string line;
    file.open("staticInit.txt");while(getline(file,line)){ Marchandise::IncNum=stoi(line);} file.close();line.clear();file.clear();
}
void copyFile(const string& source, const string& destination){
    ifstream sourceFile(source);fstream destinationFile(destination,ios::out);
    destinationFile << sourceFile.rdbuf();
    sourceFile.close();
    destinationFile.close();
}
void copyFolder(const string& source, const string& destination) {
    string command = "xcopy /s /e /i /q /y \"" + source + "\" \"" + destination + "\"";
    system(command.c_str());
}
void Backup_Protocol(){
    string sourceRef="C:\\Users\\"+string(username)+"\\MyRepo\\MiniProjet\\References.txt";
    string sourceInit="C:\\Users\\"+string(username)+"\\MyRepo\\MiniProjet\\staticInit.txt";
    string destinationRef="C:\\Users\\"+string(username)+"\\MyRepo\\MiniProjet\\BackUps\\References-Backup.txt";
    string destinationInit="C:\\Users\\"+string(username)+"\\MyRepo\\MiniProjet\\BackUps\\staticInit-backup.txt";
    string sourceFolder = "C:\\Users\\"+string(username)+"\\MyRepo\\MiniProjet\\cargaisons";
    string destinationFolder = "C:\\Users\\"+string(username)+"\\MyRepo\\MiniProjet\\BackUps\\cargaison-backup";
    thread t1(copyFile,sourceRef,destinationRef);
    thread t2(copyFile,sourceInit,destinationInit);
    thread t3(copyFolder,sourceFolder,destinationFolder);
    t1.join();t2.join();t3.join();
}
void WipeOut(){
    string sourceRef="C:\\Users\\"+string(username)+"\\MyRepo\\MiniProjet\\References.txt";
    string sourceInit="C:\\Users\\"+string(username)+"\\MyRepo\\MiniProjet\\staticInit.txt";
    string sourceFolder = "C:\\Users\\"+string(username)+"\\MyRepo\\MiniProjet\\cargaisons";
    ofstream outputFile1(sourceRef, ios::out | ios::trunc);
    ofstream outputFile2(sourceInit, ios::out | ios::trunc);
    string command = "del /s /q \"" + sourceFolder + "\"";
    system(command.c_str());

}
void Saving_Protocol(unordered_map<string,CargaisonAerienne>ca,unordered_map<string,CargaisonRoutiere>cr){

    Backup_Protocol();WipeOut();system("CLS");
    for(auto it=ca.begin();it!=ca.end();it++){FileSave(it->second);}
    for(auto it=cr.begin();it!=cr.end();it++){FileSave(it->second);}

}

int main() {

    int choice;
    unordered_set<string>Refs; vector<string>CaRefs;vector<string>CrRefs;
    unordered_map<string, CargaisonRoutiere> cargaisonsRoutieres;
    unordered_map<string, CargaisonAerienne> cargaisonsAeriennes;
    StartUp_Protocol(Refs,CaRefs,CrRefs,cargaisonsAeriennes,cargaisonsRoutieres);
    while(1){

    system("CLS");
    cout<<"\n\n\t\tGestion des Cargaisons\n\t\tCree par:Youssef Alouani & Ouissam Aboujid\n"<<endl;
    cout<<"\n\t[1]-Admin\n\t[2]-Client\nCHOICE:";
    cin>>choice;
    switch(choice){
        case 1:{
            AdminTransport admin;
            system("CLS");cout<<"\tWelcome To ADMIN INTERFACE\tNumber of unsaved Modification:"<<NumModif<<endl;
            cout<<"\n[1]-Consulter une cargaison sachant sa reference\n[2]-Consulter une marchandise sachant son numero"<<endl;
            cout<<"[3]-Consulter toutes les marchandises que contient une cargaison\n[4]-Consulter toutes les cargaisons"<<endl;
            cout<<"[5]-Consulter le poids total d'une cargaison donnee\n[6]-Consulter le cout de transport d'une cargaison donnee"<<endl;
            cout<<"[7]-Trier par ordre croissant les cargaisons par le cout de transport"<<endl;
            cout<<"[8]-Ajouter une nouvelle cargaison\n[9]-Ajouter une marchandise a une cargaison"<<endl;
            cout<<"[10]-Supprimer une marchandise d'une cargaison donnee\n[11]-Supprimer une cargaison"<<endl;
            cout<<"[12]-Modifier les informations d'une marchandise donnee"<<endl;
            cout<<"[13]-Sauvegarder Tout"<<endl;
            cout<<"\nCHOICE:";int interchoice;cin>>interchoice;
            switch(interchoice){
                case 1:{
                        system("CLS");string userRef;cout<<"\n\tEntrer La reference:";cin>>userRef;
                        if(userRef[0]=='C' && userRef[1]=='R'){admin.Consulter_Cargaison_Ref(cargaisonsRoutieres,userRef);}
                        else if(userRef[0]=='C' && userRef[1]=='A'){admin.Consulter_Cargaison_Ref(cargaisonsAeriennes,userRef);}
                        else{cout<<"\n\tInvalid Reference"<<endl;}system("pause");
                }break;
                case 2:{
                        system("CLS");int userNum;cout<<"\n\tEntrer Le Numero de marchandise:";cin>>userNum;
                        admin.Consulter_Marchandise_Num(cargaisonsAeriennes,userNum);
                        admin.Consulter_Marchandise_Num(cargaisonsRoutieres,userNum);
                        system("pause");
                }break;
                case 3:{
                        system("CLS");
                        if(!cargaisonsAeriennes.empty()||!cargaisonsRoutieres.empty()){
                            int c=1;
                            for(int i=0;i<CaRefs.size();i++){
                                cout<<"["<<c<<"]-"<<CaRefs[i]<<endl;c++;
                            }
                            for(int i=0;i<CrRefs.size();i++){
                                cout<<"["<<c<<"]-"<<CrRefs[i]<<endl;c++;
                            }

                            cout<<"\nCHOICE:";cin>>choice;
                            if(choice<=CaRefs.size()){auto it=cargaisonsAeriennes.find(CaRefs[choice-1]);it->second.afficher();}
                            else{ choice-=CaRefs.size();auto it=cargaisonsRoutieres.find(CrRefs[choice-1]);it->second.afficher();}
                        }else{cout<<"\n\tIl existe RIEN"<<endl;}
                        system("pause");
                }break;
                case 4:{
                        system("Cls");admin.Consulter_cargaisons(cargaisonsAeriennes);admin.Consulter_cargaisons(cargaisonsRoutieres);
                        system("pause");
                }break;
                case 5:{
                        system("CLS");
                        if(!cargaisonsAeriennes.empty()||!cargaisonsRoutieres.empty()){
                         int c=1;
                            for(int i=0;i<CaRefs.size();i++){
                                cout<<"["<<c<<"]-"<<CaRefs[i]<<endl;c++;
                            }
                            for(int i=0;i<CrRefs.size();i++){
                                cout<<"["<<c<<"]-"<<CrRefs[i]<<endl;c++;
                            }
                            cout<<"\nCHOICE:";cin>>choice;
                            if(choice<=CaRefs.size()){auto it=cargaisonsAeriennes.find(CaRefs[choice-1]);admin.Consulter_poids_total_cargaison(it->second);}
                            else{ choice-=CaRefs.size();auto it=cargaisonsRoutieres.find(CrRefs[choice-1]);admin.Consulter_poids_total_cargaison(it->second);}
                        }else{cout<<"\n\tIl existe RIEN"<<endl;}
                        system("pause");

                }break;
                case 6:{
                        system("CLS");
                        if(!cargaisonsAeriennes.empty()||!cargaisonsRoutieres.empty()){
                        int c=1;
                            for(int i=0;i<CaRefs.size();i++){
                                cout<<"["<<c<<"]-"<<CaRefs[i]<<endl;c++;
                            }
                            for(int i=0;i<CrRefs.size();i++){
                                cout<<"["<<c<<"]-"<<CrRefs[i]<<endl;c++;
                            }
                            cout<<"\nCHOICE:";cin>>choice;
                            if(choice<=CaRefs.size()){auto it=cargaisonsAeriennes.find(CaRefs[choice-1]);admin.Consulter_cout_transport_cargaison(it->second);}
                            else{ choice-=CaRefs.size();auto it=cargaisonsRoutieres.find(CrRefs[choice-1]);admin.Consulter_cout_transport_cargaison(it->second);}
                        }else{cout<<"\n\tIl existe RIEN"<<endl;}
                        system("pause");

                }break;
                case 7:{
                            system("CLS");
                            admin.Trier_cargaisons(cargaisonsAeriennes);admin.Trier_cargaisons(cargaisonsRoutieres);
                            cout<<"\tTOUT LES CARGAISON SONT TRIEE PAR COUT D'APRES LEUR TYPE"<<endl;
                            admin.Consulter_cargaisons(cargaisonsAeriennes);
                            admin.Consulter_cargaisons(cargaisonsRoutieres);
                            system("pause");

                }break;
                case 8:{
                        system("CLS");
                        cout<<"\tAjouter une cargaison"<<endl;
                        cout<<"\n\tType de Cargaison Aerienne/Routiere(A/R): ";char type;cin>>type;
                        if(tolower(type)=='a'){
                            cout<<"\n\tDistnace:";double dis;cin>>dis;string Nref;
                             if(!CaRefs.empty()){auto maxi=max_element(CaRefs.begin(),CaRefs.end());
                            string temp=*maxi;string temp2;
                            for(int i=2;i<temp.size();i++){temp2+=temp[i];}
                            int inc=stoi(temp2);inc++;temp2=to_string(inc);
                             Nref="CA";Nref+=temp2;}
                            else{ Nref="CA1";}
                            CargaisonAerienne c(Nref,dis);admin.Ajoute_cargaison(cargaisonsAeriennes,c);
                            CaRefs.push_back(Nref);Refs.insert(Nref);
                            cout<<"\n\tCargaison Ajoutee de reference "<<Nref<<endl;
                        }
                        else if(tolower(type)=='r'){
                            cout<<"\n\tDistnace:";double dis;cin>>dis;string Nref;
                            if(!CrRefs.empty()){auto maxi=max_element(CrRefs.begin(),CrRefs.end());
                            string temp=*maxi;string temp2;
                            for(int i=2;i<temp.size();i++){temp2+=temp[i];}
                            int inc=stoi(temp2);inc++;temp2=to_string(inc);
                            Nref="CR";Nref+=temp2;}
                            else{Nref="CR1";}
                            CargaisonRoutiere c(Nref,dis);admin.Ajoute_cargaison(cargaisonsRoutieres,c);
                            CrRefs.push_back(Nref);Refs.insert(Nref);
                            cout<<"\n\tCargaison Ajoutee de reference "<<Nref<<endl;
                        }
                        else{cout<<"\n\tType invalid"<<endl;}
                        system("pause");
                }break;
                case 9:{
                         system("CLS");cout<<"\n\tAjouter Une Marchandise a une Cargaison donnee"<<endl;
                        if(!cargaisonsAeriennes.empty()||!cargaisonsRoutieres.empty()){
                        int c=1;
                            for(int i=0;i<CaRefs.size();i++){
                                cout<<"["<<c<<"]-"<<CaRefs[i]<<endl;c++;
                            }
                            for(int i=0;i<CrRefs.size();i++){
                                cout<<"["<<c<<"]-"<<CrRefs[i]<<endl;c++;
                            }
                            cout<<"\nCHOICE:";cin>>choice;
                            cout<<"\n\tPoids:";double Poid;cin>>Poid;
                            cout<<"\n\tVolume:";double vol;cin>>vol;
                            if(choice<=CaRefs.size()){Marchandise m(Poid,vol);admin.Ajoute_Marchandise(cargaisonsAeriennes,CaRefs[choice-1],m);}
                            else{ choice-=CaRefs.size();
                                Marchandise m(Poid,vol);admin.Ajoute_Marchandise(cargaisonsRoutieres,CrRefs[choice-1],m);
                            }
                        }else{cout<<"\n\tIl existe RIEN"<<endl;}
                        system("pause");
                }break;
                case 10:{
                        system("CLS");
                        if(!cargaisonsAeriennes.empty()||!cargaisonsRoutieres.empty()){
                            int c=1;
                            for(int i=0;i<CaRefs.size();i++){
                                cout<<"["<<c<<"]-"<<CaRefs[i]<<endl;c++;
                            }
                            for(int i=0;i<CrRefs.size();i++){
                                cout<<"["<<c<<"]-"<<CrRefs[i]<<endl;c++;
                            }
                            cout<<"\nCHOICE:";cin>>choice;
                            if(choice<=CaRefs.size()){
                                    auto it=cargaisonsAeriennes.find(CaRefs[choice-1]);it->second.afficher();
                                    cout<<"\nNumero de marchandise:";int userNum;cin>>userNum;
                                    admin.suppression_marchandise(cargaisonsAeriennes,CaRefs[choice-1],userNum);
                                    }
                            else{choice-=CaRefs.size();
                            auto it=cargaisonsRoutieres.find(CrRefs[choice-1]);it->second.afficher();
                            cout<<"\nNumero de marchandise:";int userNum;cin>>userNum;
                            admin.suppression_marchandise(cargaisonsRoutieres,CrRefs[choice-1],userNum);
                            }
                        }else{cout<<"\n\tIl existe RIEN"<<endl;}
                        system("pause");


                }break;
                case 11:{
                    system("CLS");
                        if(!cargaisonsAeriennes.empty()||!cargaisonsRoutieres.empty()){
                        int c=1;
                            for(auto i=0;i<CaRefs.size();i++){
                                cout<<"["<<c<<"]-"<<CaRefs[i]<<endl;c++;
                            }
                            for(int i=0;i<CrRefs.size();i++){
                                cout<<"["<<c<<"]-"<<CrRefs[i]<<endl;c++;
                            }
                            cout<<"\nCHOICE:";cin>>choice;
                            if(choice<=CaRefs.size()){
                                    string cargo=CaRefs[choice-1];cout<<"cargo:"<<cargo<<endl;
                                    admin.suppression_Cargo(cargaisonsAeriennes,cargo);
                                    auto it=Refs.find(cargo);Refs.erase(it);
                                    auto i=find(CaRefs.begin(),CaRefs.end(),cargo);CaRefs.erase(i);

                            }
                            else{ choice-=CaRefs.size();
                                    string cargo=CrRefs[choice-1];
                                    admin.suppression_Cargo(cargaisonsRoutieres,cargo);
                                    auto it=Refs.find(cargo);Refs.erase(it);
                                    auto i=find(CrRefs.begin(),CrRefs.end(),cargo);CrRefs.erase(i);

                            }
                        }else{cout<<"\n\tIl existe RIEN"<<endl;}

                        system("pause");
                }break;
                case 12:{
                    system("CLS");
                        if(!cargaisonsAeriennes.empty()||!cargaisonsRoutieres.empty()){
                            int c=1;
                            for(int i=0;i<CaRefs.size();i++){
                                cout<<"["<<c<<"]-"<<CaRefs[i]<<endl;c++;
                            }
                            for(int i=0;i<CrRefs.size();i++){
                                cout<<"["<<c<<"]-"<<CrRefs[i]<<endl;c++;
                            }
                            cout<<"\nCHOICE:";cin>>choice;
                            if(choice<=CaRefs.size()){
                                    auto it=cargaisonsAeriennes.find(CaRefs[choice-1]);it->second.afficher();
                                    cout<<"\nNumero de marchandise:";int userNum;cin>>userNum;
                                    admin.modifier_marchandise(cargaisonsAeriennes,CaRefs[choice-1],userNum);
                                    }
                            else{choice-=CaRefs.size();
                            auto it=cargaisonsRoutieres.find(CrRefs[choice-1]);it->second.afficher();
                            cout<<"\nNumero de marchandise:";int userNum;cin>>userNum;
                            admin.modifier_marchandise(cargaisonsRoutieres,CrRefs[choice-1],userNum);
                            }
                        }else{cout<<"\n\tIl existe RIEN"<<endl;}
                        system("pause");
                }break;
                case 13:{Saving_Protocol(cargaisonsAeriennes,cargaisonsRoutieres);cout<<"\n\tSauvegarde Complet"<<endl;NumModif=0;system("pause");}break;
          }

        }break;
        case 2:{
            AdminTransport admin;
            system("CLS");cout<<"\tWelcome To Client INTERFACE"<<endl;
            cout<<"\n[1]-Consulter une cargaison sachant sa reference\n[2]-Consulter une marchandise sachant son numero"<<endl;
            cout<<"[3]-Consulter toutes les marchandises que contient une cargaison\n[4]-Consulter toutes les cargaisons"<<endl;
            cout<<"[5]-Consulter le poids total d'une cargaison donnee\n[6]-Consulter le cout de transport d'une cargaison donnee"<<endl;
            cout<<"[7]-Trier par ordre croissant les cargaisons par le cout de transport"<<endl;
             cout<<"\nCHOICE:";int interchoice;cin>>interchoice;
            switch(interchoice){
                case 1:{
                        system("CLS");string userRef;cout<<"\n\tEntrer La reference:";cin>>userRef;
                        if(userRef[0]=='C' && userRef[1]=='R'){admin.Consulter_Cargaison_Ref(cargaisonsRoutieres,userRef);}
                        else if(userRef[0]=='C' && userRef[1]=='A'){admin.Consulter_Cargaison_Ref(cargaisonsAeriennes,userRef);}
                        else{cout<<"\n\tInvalid Reference"<<endl;}system("pause");
                }break;
                case 2:{
                        system("CLS");int userNum;cout<<"\n\tEntrer Le Numero de marchandise:";cin>>userNum;
                        admin.Consulter_Marchandise_Num(cargaisonsAeriennes,userNum);
                        admin.Consulter_Marchandise_Num(cargaisonsRoutieres,userNum);
                        system("pause");
                }break;
                case 3:{
                        system("CLS");
                        if(!cargaisonsAeriennes.empty()||!cargaisonsRoutieres.empty()){
                            int c=1;
                            for(int i=0;i<CaRefs.size();i++){
                                cout<<"["<<c<<"]-"<<CaRefs[i]<<endl;c++;
                            }
                            for(int i=0;i<CrRefs.size();i++){
                                cout<<"["<<c<<"]-"<<CrRefs[i]<<endl;c++;
                            }
                            cout<<"\nCHOICE:";cin>>choice;
                            if(choice<=CaRefs.size()){auto it=cargaisonsAeriennes.find(CaRefs[choice-1]);it->second.afficher();}
                            else{ choice-=CaRefs.size();auto it=cargaisonsRoutieres.find(CrRefs[choice-1]);it->second.afficher();}
                        }else{cout<<"\n\tIl existe RIEN"<<endl;}
                        system("pause");
                }break;
                case 4:{
                        system("Cls");admin.Consulter_cargaisons(cargaisonsAeriennes);admin.Consulter_cargaisons(cargaisonsRoutieres);
                        system("pause");
                }break;
                case 5:{
                        system("CLS");
                        if(!cargaisonsAeriennes.empty()||!cargaisonsRoutieres.empty()){
                         int c=1;
                            for(int i=0;i<CaRefs.size();i++){
                                cout<<"["<<c<<"]-"<<CaRefs[i]<<endl;c++;
                            }
                            for(int i=0;i<CrRefs.size();i++){
                                cout<<"["<<c<<"]-"<<CrRefs[i]<<endl;c++;
                            }
                            cout<<"\nCHOICE:";cin>>choice;
                            if(choice<=CaRefs.size()){auto it=cargaisonsAeriennes.find(CaRefs[choice-1]);admin.Consulter_poids_total_cargaison(it->second);}
                            else{ choice-=CaRefs.size();auto it=cargaisonsRoutieres.find(CrRefs[choice-1]);admin.Consulter_poids_total_cargaison(it->second);}
                        }else{cout<<"\n\tIl existe RIEN"<<endl;}
                        system("pause");

                }break;
                case 6:{
                        system("CLS");
                        if(!cargaisonsAeriennes.empty()||!cargaisonsRoutieres.empty()){
                        int c=1;
                            for(int i=0;i<CaRefs.size();i++){
                                cout<<"["<<c<<"]-"<<CaRefs[i]<<endl;c++;
                            }
                            for(int i=0;i<CrRefs.size();i++){
                                cout<<"["<<c<<"]-"<<CrRefs[i]<<endl;c++;
                            }
                            cout<<"\nCHOICE:";cin>>choice;
                            if(choice<=CaRefs.size()){auto it=cargaisonsAeriennes.find(CaRefs[choice-1]);admin.Consulter_cout_transport_cargaison(it->second);}
                            else{ choice-=CaRefs.size();auto it=cargaisonsRoutieres.find(CrRefs[choice-1]);admin.Consulter_cout_transport_cargaison(it->second);}
                        }else{cout<<"\n\tIl existe RIEN"<<endl;}
                        system("pause");

                }break;
                case 7:{
                            system("CLS");
                            admin.Trier_cargaisons(cargaisonsAeriennes);admin.Trier_cargaisons(cargaisonsRoutieres);
                            cout<<"\tTOUT LES CARGAISON SONT TRIEE PAR COUT D'APRES LEUR TYPE"<<endl;
                            admin.Consulter_cargaisons(cargaisonsAeriennes);
                            admin.Consulter_cargaisons(cargaisonsRoutieres);
                            system("pause");

                }break;}
        }break;

    }


    }return 0;
}





