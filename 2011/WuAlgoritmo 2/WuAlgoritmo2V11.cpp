#include <iostream>
#include <list>
#include <vector>
#include <map>
#include <fstream>
#include <stack>
#include <float.h>

using namespace std;

/*
* - 16/04/2012: correcao em Steps1and2().
*				+ if (maiorNivel > nivelMaisProfundo) { // "maiorNivel" era "indiceMaiorNivel"
*					// Atualizar nivel mais profundo ja visto
*					nivelMaisProfundo = maiorNivel; // "maiorNivel" era "indiceMaiorNivel"
*               + Precisava desta inicializacao tambem: int indiceMaiorNivel = i;
*
* - 17/04/2012: correcao em Step3().
*   Ao verificar insercao do novo vertice no "meio" da extensao parcial, podemos decidir por
*   desistir da busca, caso nao seja possivel inserir "novo" na posicao atual ou subsequente, ou
*   apenas resetar o melhor custo encontrado ate agora, caso nos deparemos com um elemento que
*   precisar ficar antes de "novo" (neste caso, nenhuma das posicoes anteriores era valida).
*/

class Vertice;

class Arco {
private:
	class Vertice *U;
    class Vertice *V;

public:
	Arco(Vertice *u, Vertice *v) {
		U = u;
		V = v;
    }

    Arco(Arco *a) {
		U = a->getInicio();
		V = a->getFinal();
    }

	Vertice * getInicio() {
		return U;
	}

	Vertice * getFinal() {
		return V;
	}
};

class Vertice {
private:
	int Indice;
	int GrauDeEntrada;
	int GrauDeSaida;
	list<Arco*> *ListaDeAdjacencia;
	list<Arco*> *ListaDeIncidencia;
	vector<double> *Custo;
	vector<bool> *NoDownset;

public:
    Vertice(int i, int totalDeNos){
		Indice = i;
		GrauDeEntrada = 0;
		GrauDeSaida = 0;
		ListaDeAdjacencia = new list<Arco*>;
		ListaDeIncidencia = new list<Arco*>;
		Custo = new vector<double>(totalDeNos);
		NoDownset = new vector<bool>(totalDeNos);
		for (int i=0; i<totalDeNos; i++) {
			Custo->at(i) = DBL_MAX;
			NoDownset->at(i) = false;
		}
	}

	void adicionarCusto(Vertice* vertice, double custo){
		Custo->at(vertice->getIndice()) = custo;
		NoDownset->at(vertice->getIndice()) = false;
	}

	inline void adicionarRelacao(Vertice* vertice){
		NoDownset->at(vertice->getIndice()) = true;
		Custo->at(vertice->getIndice()) = 0.0;
	}

	inline void adicionarRelacao(int v){
		NoDownset->at(v) = true;
		Custo->at(v) = 0.0;
	}

	/*
	* Agora esta funcao retorna true se o parametro v for comparavel com o vertice "this".
	* O retorno "false" so acontece se "this" estiver no downset de v.
	*/
	bool obterCustoParaVertice(int v, double &custo) {
		if (NoDownset->at(v)) {
			custo = 0.0;
			return true;
		}
		else {
			custo = Custo->at(v);
			if (custo < DBL_MAX - 1.0)
				return true;
			else // "this" esta' no downset de "v"
				return false;
		}
	}

	inline bool estaNoDownset(int v) {
		return NoDownset->at(v);
	}

	void adicionarArcoDeSaida(Arco *a) {
         Arco *A = new Arco(a);
         ListaDeAdjacencia->push_back(A);
		 adicionarRelacao(a->getFinal());
	}

	void adicionarArcoDeSaida(Arco *a, double custo) {
         Arco *A = new Arco(a);
		 adicionarCusto(a->getFinal(),custo);
	}

	void adicionarArcoDeEntrada(Arco *a) {
         Arco *A = new Arco(a);
         ListaDeIncidencia->push_back(A);
	}

	void removerArcoDeSaida(Arco *a) {
		ListaDeAdjacencia->remove(a);
	}

	int  getGrauDeEntrada() {
		return GrauDeEntrada;
	}

	void incrementarGrauDeEntrada() {
		GrauDeEntrada ++;
	}

	void decrementarGrauDeEntrada() {
		GrauDeEntrada --;
	}

	int  getGrauDeSaida() {
		return GrauDeSaida;
	}

	void incrementarGrauDeSaida() {
		GrauDeSaida ++;
	}

	void decrementarGrauDeSaida() {
		GrauDeSaida --;
	}

	int  getIndice() {
		return Indice;
	}

	list<Arco*>* getListaDeAdjacencia() {
		return ListaDeAdjacencia;
	}

	list<Arco*>* getListaDeincidencia() {
		return ListaDeIncidencia;
	}
};

class GrafoDirecionado {
private:
	vector<Vertice*> *Vertices;
	list<Arco*> *Arcos;

public:
	GrafoDirecionado(int numVertices){
		Vertices = new vector<Vertice*>(numVertices);
		for (int i = 0; i < numVertices; i++) {
			Vertices->at(i) = new Vertice(i, numVertices);
		}
		Arcos = new list<Arco*>();
	}

	int getNumeroDeVertices() {
		return Vertices->size();
	}

	int getNumeroDeArcos() {
		return Arcos->size();
	}

	Vertice* getVertice(int i) {
		if (i < getNumeroDeVertices())
			return Vertices->at(i);
		return NULL;
	}

	Arco* adicionarArco(int origem, int destino) {
		int tamanho = getNumeroDeVertices();
		if ((origem < tamanho) && (destino < tamanho)) {
			Vertice *verticeOrigem = getVertice(origem);
			Vertice *verticeDestino = getVertice(destino);
			if ((verticeOrigem != NULL) && (verticeDestino != NULL)) {
				for(list<Arco*>::iterator it = verticeDestino->getListaDeAdjacencia()->begin(); it != verticeDestino->getListaDeAdjacencia()->end();it++)
					if((*it)->getInicio() == verticeOrigem)
						return NULL;
				Arco *arco = new Arco(verticeOrigem , verticeDestino);
				verticeOrigem->adicionarArcoDeSaida(arco);
				verticeOrigem->incrementarGrauDeSaida();
				verticeDestino->adicionarArcoDeEntrada(arco);
				verticeDestino->incrementarGrauDeEntrada();
				Arcos->push_back(arco);
				return arco;
			}
		}
		return NULL;
	}

	// Adiciona apenas os arcos de custos (serao invisiveis ao Grafo original)
	Arco* adicionarArcoDeCusto(int origem, int destino, double custo) {
		int tamanho = getNumeroDeVertices();
		if ((origem < tamanho) && (destino < tamanho)) {
			Vertice *verticeOrigem = getVertice(origem);
			Vertice *verticeDestino = getVertice(destino);
			if ((verticeOrigem != NULL) && (verticeDestino != NULL)) {
				for(list<Arco*>::iterator it = verticeDestino->getListaDeAdjacencia()->begin(); it != verticeDestino->getListaDeAdjacencia()->end();it++)
					if((*it)->getInicio() == verticeOrigem)
						return NULL;
				Arco *arco = new Arco(verticeOrigem , verticeDestino);
				verticeOrigem->adicionarArcoDeSaida(arco, custo);
				Arcos->push_back(arco);
				return arco;
			}
		}
		return NULL;
	}

	void removerArco(Arco *arco) {
		Vertice *fim = arco->getFinal();
		fim->removerArcoDeSaida(arco);
		Vertice *inicio = arco->getInicio();
		inicio->decrementarGrauDeEntrada();
		fim->decrementarGrauDeSaida();
		Arcos->remove(arco);
	}

	list<Arco*>* getArcos(){
		return Arcos;
	}

	vector<Vertice*>* getVertices(){
		return Vertices;
	}

};

class Poset : public GrafoDirecionado {

public:
	Poset(int numVertices)
		: GrafoDirecionado(numVertices) {}

	// Obtem a cadeia mais longa do poset sem efetuar a decomposicao em niveis descrita em Wu et al.
	// Alcanca o objetivo dos passos 1 e 2 do algoritmo 2 de Wu et al., com a mesma complexidade assintotica.
	list<Vertice*>* Steps1And2() {

		int n = getNumeroDeVertices();
		list<Vertice*> *cadeiaMaisLonga = new list<Vertice*>;
		int nivelMaisProfundo = -1;

		// Para cada vertice com grau de entrada zero, executar DFS
		for (int i = 0 ; i < getNumeroDeVertices() ; i++ ) {
			if (getVertice(i)->getGrauDeEntrada() == 0) {

				int *pred = new int[n];
				pred[i] = i;
				stack<Vertice*> pilha;
				pilha.push(getVertice(i));
				Vertice *vertice;
				vector<bool> visitados;
				visitados.resize(n);
				visitados.at(i) = true;

				// Efetua DFS sobre um vertice de grau zero
				vector<int> nivel;
				nivel.resize(n);
				nivel.at(i) = 0;
				int maiorNivel = 0;
				int indiceMaiorNivel = i;

				while (!pilha.empty()) {
					vertice = pilha.top();
					pilha.pop();
					// Percorre cada vizinho nao visitado
					for (list<Arco*>::iterator it = vertice->getListaDeAdjacencia()->begin(); it != vertice->getListaDeAdjacencia()->end(); it++) {
						if (visitados.at((*it)->getFinal()->getIndice()) == false) {
							pilha.push((*it)->getFinal());
							int indiceFinal = (*it)->getFinal()->getIndice();
							visitados.at(indiceFinal) = true;
							// Atualiza informacoes de nivel e predecessor
							pred[indiceFinal] = (*it)->getInicio()->getIndice();
							nivel[indiceFinal] = nivel[(*it)->getInicio()->getIndice()]+1;
							// Se nivel do vertice final for maior que o maior nivel ja visto, atualizar
							if (nivel[indiceFinal]  >= maiorNivel) {
								maiorNivel = nivel[indiceFinal];
								indiceMaiorNivel = indiceFinal;
							}
						}
					}
				}//while

				if (maiorNivel > nivelMaisProfundo) {
					// Atualizar nivel mais profundo ja visto
					nivelMaisProfundo = maiorNivel;
					// Percorrer vetor pred 'backwards', reconstruindo a cadeia mais longa
					int indiceAux = indiceMaiorNivel;
					cadeiaMaisLonga->clear();
					cadeiaMaisLonga->push_front(getVertice(indiceMaiorNivel));
					while (pred[indiceAux] !=  indiceAux) {
						indiceAux = pred[indiceAux];
						cadeiaMaisLonga->push_front(getVertice(indiceAux));
					}
				}
			}//if
		}//for

		//Armazenar explicitamente todas as rela��es transitivas
		OrdTopologica();

		return cadeiaMaisLonga;
	}

	// Exibe o conteudo de uma cadeia, representada como uma lista
	void exibirCadeia(list<Vertice*> *extlinear){
		for(list<Vertice*>::iterator it = extlinear->begin(); it != extlinear->end(); it++)
			cout << (*it)->getIndice() << " ";
		cout << endl;
	}

	// Passo 3 do algoritmo 2 de Wu et al.
	void Step3(list<Vertice*> *extensao){


		list<Vertice*> verticesNaoIncluidos;
		list<Vertice*>::iterator iterVertice;
		list<Vertice*>::iterator posicaoDeInsercao;
		double custoTotal = 0;

		// Popular lista de vertices que ainda nao pretencem aa extensao
		for (int i = 0 ; i < getNumeroDeVertices() ; i++)
			verticesNaoIncluidos.push_back(getVertice(i));
		for (iterVertice = extensao->begin() ; iterVertice != extensao->end() ; iterVertice++)
			verticesNaoIncluidos.remove(*iterVertice);

		// Incluir cada vertice na extensao, seguindo a ordem da lista
		while (verticesNaoIncluidos.size() > 0) {
			Vertice *novo = verticesNaoIncluidos.front();
			verticesNaoIncluidos.pop_front();

			double custoNovoVertice = DBL_MAX, custoNaPosicao;
			for (iterVertice = extensao->begin() ; iterVertice != extensao->end() ; iterVertice++) {

				// Verificar insercao do novo vertice no inicio da extensao parcial
				if (iterVertice == extensao->begin()) {
					if (novo->obterCustoParaVertice((*iterVertice)->getIndice(), custoNaPosicao)) {
						custoNovoVertice = custoNaPosicao;
						posicaoDeInsercao = extensao->begin();
					}
				}
				// Verificar insercao do novo vertice no final da extensao parcial
				else if ((*iterVertice) == extensao->back()) {
					if ((*iterVertice)->obterCustoParaVertice(novo->getIndice(), custoNaPosicao) && (custoNaPosicao < custoNovoVertice)) {
						custoNovoVertice = custoNaPosicao;
						posicaoDeInsercao = extensao->end();
					}
				}
				// Verificar insercao do novo vertice no "meio" da extensao parcial
				else {
					// Inclui logica de "desistir", caso nao seja possivel inserir "novo" na posicao atual ou subsequente
					double custoComPosterior, custoComAnterior;
					if (novo->obterCustoParaVertice((*iterVertice)->getIndice(), custoComPosterior)) {
						// Obter referencia para vertice anterior na extensao parcial
						list<Vertice *>::iterator anterior = iterVertice;
						anterior--;
						if ((*anterior)->obterCustoParaVertice(novo->getIndice(), custoComAnterior)) {
							custoNaPosicao = custoComAnterior + custoComPosterior;
							if (custoNaPosicao < custoNovoVertice){
								custoNovoVertice = custoNaPosicao;
								posicaoDeInsercao = iterVertice;
							}
						}
						else { // "novo" nao pode ficar abaixo de "anterior".
							// Precisamos interromper a busca
							break;
						}
					}
					else { // "novo" deve ficar abaixo de "*iterVertice"
						// Resetar melhor custo visto ate agora (as posicoes anteriores eram todas invalidas)
						custoNovoVertice = DBL_MAX;
					}
				}
			}//for

			cout << "Menor custo: " << custoNovoVertice << endl;
			extensao->insert(posicaoDeInsercao, novo);
			exibirCadeia(extensao);
			cout << endl;
			custoTotal += custoNovoVertice;
		}
		cout << "O custo total da extensao linear e: " << custoTotal << endl;
	}

	void OrdTopologica(){

		//Descobre o maior grau
		int maiorGrau = -1;
		for(vector<Vertice*>::iterator it = getVertices()->begin()++;it != getVertices()->end();it++){
			if((*it)->getGrauDeSaida() > maiorGrau)
				maiorGrau = (*it)->getGrauDeSaida();
		}

		//Preenche as "caixas"
		int *NaCaixa = new int[getNumeroDeVertices()];
		vector<list<Vertice*>> *caixas = new vector<list<Vertice*>>(maiorGrau+1);
		for(vector<Vertice*>::iterator it = getVertices()->begin(); it != getVertices()->end(); it++) {
			int indiceDaCaixa = (*it)->getGrauDeSaida();
			caixas->at(indiceDaCaixa).push_back((*it));
			NaCaixa[(*it)->getIndice()] = indiceDaCaixa;
		}

		//Ordena��o topologica por caixas
		while(caixas->at(0).size() > 0) {
			Vertice * verticeGrauZero = caixas->at(0).front();
			caixas->at(0).pop_front();
			for (list<Arco*>::iterator it = verticeGrauZero->getListaDeincidencia()->begin();
				it != verticeGrauZero->getListaDeincidencia()->end(); it++) {

					//copia os transitivos
					list<Arco*>::iterator iterador = (*it)->getFinal()->getListaDeAdjacencia()->begin();
					while(iterador != (*it)->getFinal()->getListaDeAdjacencia()->end()){
						(*it)->getInicio()->adicionarRelacao((*iterador)->getFinal()->getIndice());
						iterador++;
					}

					int ondeInicioEsta = NaCaixa[(*it)->getInicio()->getIndice()];
					caixas->at(ondeInicioEsta).remove((*it)->getInicio());
					caixas->at(ondeInicioEsta-1).push_back((*it)->getInicio());
					NaCaixa[(*it)->getInicio()->getIndice()] = ondeInicioEsta-1;

			}
		}

	}

};

int main() {

	int NumeroDeVertices, NumeroDeArcosDiretos, NumeroDeArcosComCustos;

	ifstream fin;
	//fin.open("Grafo.txt");
	fin.open("poset_n10_d4_s345.txt");
	fin >> NumeroDeVertices;
	fin >> NumeroDeArcosDiretos;
	fin >> NumeroDeArcosComCustos;

    Poset p(NumeroDeVertices);

	int aux1,aux2,aux3;
	for (int i = 0; i < NumeroDeArcosDiretos; i++) {
		fin >> aux1;
		fin >> aux2;
		p.adicionarArco( aux1 , aux2 );
	}

	for (int i = 0; i < NumeroDeArcosComCustos; i++) {
		fin >> aux1;
		fin >> aux2;
		fin >> aux3;
		p.adicionarArcoDeCusto( aux1 , aux2 , aux3 );
	}

	fin.close();

	list<Vertice*> *ext = p.Steps1And2();
	p.exibirCadeia(ext);

	p.Step3(ext);
	cout << endl;
	list<int> teste;
	teste.push_back(1);
	teste.push_back(1);
	teste.push_back(1);
	teste.insert(teste.begin(),2);
	teste.insert(teste.end(),6);
	for(list<int>::iterator it = teste.begin(); it != teste.end() ; it++)
		cout << (*it) << endl;
	getchar();
}
