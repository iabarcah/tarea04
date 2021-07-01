#include <boost/mpi.hpp>
#include <iostream>
#include <random>
#include <thread> 
#include <chrono>
#include <cmath>
#include <cstring>


namespace mpi = boost::mpi;

void createNumbers(const int& count, float* out)
{
	std::random_device rd;  //seed for the random number engine
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_real_distribution<> distrib(0, 10000);

	for(size_t i=0; i < count; i++){
		out[i] = (distrib(gen));
	}
}

void uso(std::string pname)
{
	std::cerr << "Uso: " << pname << " --ntotal cantidad_nros_a_sumar" << std::endl;
	exit(EXIT_FAILURE);
}

/*std::int min(const std::int &lhs, const std::int &rhs)
{
  return lhs < rhs ? lhs : rhs;
}*/
bool comp(int a, int b)
{
    return (a < b);
}
int main(int argc, char *argv[])
{
	mpi::timer T;
	mpi::environment env{argc, argv};
	mpi::communicator world;
	
	int ntotal = 100;
	int rankID = world.rank();
	int totalProcInComm = world.size();
	
	float sumaSecuencial = 0.0;
	double timeStamp, tiempoSecuencial, tiempoParalelo;
	
	
	//////////////////////////////////////////
	//  Read command-line parameters easy way
	if(argc != 3){
		uso(argv[0]);
	}
	std::string mystr;
	for (size_t i=0; i < argc; i++) {
		mystr=argv[i];
		if (mystr == "--ntotal") {
			ntotal = std::atoi(argv[i+1]);
		}
	}

	
	float* numbersToShare;
	numbersToShare = new float[ntotal];
	
	//El proceso 0:
	//    crea los números.
	//    hace la suma secuencial. Este valor se usa para comprobar el cómputo paralelo.
	//    reparte los números creados a los procesos que están en el communicator.
	if(rankID == 0){
		
		createNumbers(ntotal, numbersToShare);
		std::cout << T.elapsed() << "> " << rankID << ": datos creados" << std::endl;
	
		
		timeStamp = T.elapsed();
		for(size_t idx = 0; idx < ntotal; idx++){
			//std::cout  << rankID << ":" << numbersToShare[idx] << std::endl;
			sumaSecuencial += std::sin((numbersToShare[idx]))*std::cos((numbersToShare[idx]));
		}
	
	
		tiempoSecuencial = T.elapsed() - timeStamp;
	
		std::cout << T.elapsed() << "> " << rankID << ": Suma secuencial: " << sumaSecuencial << std::endl;
		std::cout << T.elapsed() << "> " << rankID << ": Tiempo secuencial: " << tiempoSecuencial << std::endl;
	}
	
	timeStamp = T.elapsed();
	//
	//cada proceso prepara el contenedor que va a recibir los números del rankID 0.
	//
	int ntotalByProc = ntotal/totalProcInComm;
	float* numbersToSum;
	numbersToSum = new float[ntotalByProc];
	
	// ###############Completar código############
	// El proceso 0 reparte los números con scatter
	int senderRank = 0;
	// defininicón de mpi::scatter 
	//   https://www.boost.org/doc/libs/1_71_0/doc/html/boost/mpi/scatter.html
	//mpi::scatter(world, dataToShare, dataByProc, ntotalByProc, senderRank);
	
	//Cada procesa realiza su suma parcial
	float sumaParcial = 0.0;
	mpi::scatter(world,numbersToShare,numbersToSum,ntotalByProc,senderRank);
	for(size_t idx = 0; idx < ntotalByProc; idx++){
		sumaParcial += std::sin((numbersToSum[idx]))*std::cos((numbersToSum[idx]));
		
	}
	//mpi::scatter(world, dataToShare, dataByProc, , senderRank);
	
	// ###############Completar código############
	//Los procesos envian sus resultados parciales para que el proceso 0 haga la reducción
	float sumaParalela = 0.0;
	// definición de mpi::reduce
	if (world.rank() == 0) {
    	int minimum;
		bool var= comp(0,10000);
    	reduce(world, numbersToSum,sumaParalela, std::plus<int>() ,0);
    	std::cout << "The sum value is " << sumaParalela << std::endl;
  } else {
    	//reduce(world, my_number, mpi::minimum<int>(), 0);
  }
	//  https://www.boost.org/doc/libs/1_71_0/doc/html/boost/mpi/reduce.html


	//Finalmente, el proceso 0 muestra la suma paralela y el tiempo que tomó
	//realizarla.
	if(world.rank() == 0) {
		tiempoParalelo = T.elapsed() - timeStamp;
		std::cout  << T.elapsed() << ">" << rankID << ": Suma Paralela:" << sumaParalela << std::endl;
		std::cout << T.elapsed() << "> " << rankID << ": Tiempo paralelo: " << tiempoParalelo << std::endl;
	}
	
	

	delete[] numbersToShare;
	return(EXIT_SUCCESS);
}
