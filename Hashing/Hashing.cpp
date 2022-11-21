// Hashing.cpp : Este arquivo contém a função 'main'. A execução do programa começa e termina ali.
//
#include<fstream>
#include<iostream>
#include<sstream>
#include<string>
#include<list>
#include<algorithm>
#include<cstdlib>

class Aluno {
    const std::wstring nome;
public:
    Aluno(const __int32 matricula, const std::wstring nome):matricula(matricula),nome(nome) {}
    Aluno(const std::string matricula, const std::wstring nome) :Aluno(std::stoi(matricula), nome) {}
    const __int32 matricula;
    void Print() const {
        std::wcout << "Nome: " << nome << "  || Matricula: " << matricula;
    }
    int Hash() const {
        return Aluno::Hash(matricula);
    }
    static __int32 Hash(__int32 mat) {
        auto x = mat;
        x = ((x >> 16) ^ x) * 0x45d9f3b;
        x = ((x >> 16) ^ x) * 0x45d9f3b;
        x = (x >> 16) * 0x45d9f3b;
        return abs(x);
    }
    bool operator==(const Aluno& al) const {
        if (al.matricula == matricula) return true;
        return false;
    }
    bool operator!=(const Aluno& al) const {
        return !(*this == al);
    }
};

class HashTable {
    std::list<Aluno>* table;
    const int col_size;
    int TrueHash(const Aluno& a) const {
        return a.Hash() % col_size;
    }
    int TrueHash(int matricula) const {
        return Aluno::Hash(matricula) % col_size;
    }
public:
    HashTable(int bucketNum):col_size(bucketNum) {
        table = new std::list<Aluno>[bucketNum];
    }
    HashTable(int numeroTotal, float desiredLoadFactor) : HashTable(static_cast<int>(numeroTotal / desiredLoadFactor)) {}
    ~HashTable() {
        delete[] table;
    }
    void Insert(const Aluno& element) {
        table[TrueHash(element)].push_front(element);
    }
    void Remove(const Aluno& element) {
        table[TrueHash(element)].remove(element);
    }
    void Remove(std::string matricula) {
        Remove(std::stoi(matricula));
    }
    void Remove(int matricula) {
        table[TrueHash(matricula)].remove_if(
            [=](Aluno& a) {
                return (a.matricula == matricula) ? true : false; 
            }
        );
    }
    Aluno& Find(std::string matricula) const {
        return Find(std::stoi(matricula));
    }
    Aluno& Find(int matricula) const {
        auto num=TrueHash(matricula);
        for (auto i = table[num].begin(); i != table[num].end(); ++i) if (i->matricula == matricula) return *i;
        throw std::exception("Aluno com esta matricula nao foi encontrado");
    }
    void Print() const {
        for (int i = 0; i < col_size; ++i) {
            std::cout << "Lista " << i << ": \n";
            for (auto a : table[i]) {
                a.Print();
                std::cout << '\n';
            }
            std::cout << "\n\n\n\n";
        }
    }
    void PrintNum() const {
        unsigned numeros[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
        for (int i = 0; i < col_size; ++i) {
            auto s = table[i].size();
            if (s < 11) ++numeros[s];
            else ++(numeros[11]);
        }
        for (int i = 0; i < 11; ++i) {
            std::cout << "Buckets com " << i << " elementos: " << numeros[i] << '\n';
        }
        std::cout << "Buckets com mais de 10 elementos:" << numeros[11] << '\n';
    }
};

std::wifstream arquivo("C:\\Users\\farof\\Desktop\\listadealunos.csv"); //coloque aqui o caminho pra o arquivo
void close() {
    arquivo.close();
}

int main()
{
    std::atexit(close);
    constexpr int NUMALUNOS = 2357;
    HashTable tabela(NUMALUNOS, 0.25); //segundo argumento é o fator de carga, insira ali o fator de carga desejado
    std::wstring s;
    while (getline(arquivo,s)) {
        std::wstringstream ss(s);
        int matricula;
        std::wstring nome;
        ss >> matricula;
        wchar_t c;
        ss >> c;
        getline(ss, nome);
        Aluno a(matricula, nome);
        tabela.Insert(a);
    }
    tabela.Print();
    std::cout << '\n';
    tabela.PrintNum(); //isso aqui vai dizer o numero de buckets com 0 elemento, 1 elemento, 2 elementos etc.
    char c;
    std::cin >> c;
    return 0; 
}
