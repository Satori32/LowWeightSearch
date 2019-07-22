#include <iostream>
#include <vector>
#include <cstdint>
#include <tuple> 
#include <algorithm>
#include <array>


typedef std::vector<std::vector<std::uint64_t>> MapType;
typedef std::tuple<std::int64_t, std::int64_t> Point;
typedef std::vector<Point> FootPrint;

int Pa = 2;

std::uintmax_t PoorSearch(const MapType& M, const Point& Fst, const Point& Last) {

	std::uintmax_t W = 0;
	std::int64_t i = 0;
	std::size_t j = 0;
	for (j= std::get<1>(Fst);j != std::get<1>(Last); j += (std::get<1>(Fst) - std::get<1>(Last)) ? 1 : -1) {
		W += M[j][std::get<0>(Fst)];
	}

	for (i = std::get<0>(Fst); i != std::get<0>(Last); i += (std::get<0>(Fst) - std::get<0>(Last)) ? 1 : -1) {
		W += M[j-1][i];
	}
	
	return W;
}
/**/
bool SearchPath_r(const MapType& M, FootPrint FP, const Point& Now, const Point& Last, const std::uintmax_t& Wait, std::intmax_t& Min,FootPrint& FPR) {
	static std::array<int, 4> XP = { 0,1,0,-1 };
	static std::array<int, 4> YP = { 1,0,-1,0 };
	static std::uintmax_t C = 0;

	auto X1 = std::get<0>(Now);
	auto Y1 = std::get<1>(Now);
	auto X2 = std::max<int>(std::get<0>(Last), 0);
	auto Y2 = std::max<int>(std::get<1>(Last), 0);
	if ((X1 == X2) && (Y1 == Y2)) {
		if (Min >= Wait) {
			FP.push_back({ X1,Y1 });
			Min = Wait;
			C++;	
			FPR = FP;
		}
	}

	if (Min <= Wait) {
		return false;
	}
	FP.push_back({ std::get<0>(Now),std::get<1>(Now) });
	for (std::size_t i = 0; i < 4; i++) {
		if (std::get<0>(Now) + XP[i] < 0) { continue; };
		if (std::get<1>(Now) + YP[i] < 0) { continue; };
		if (std::get<0>(Now) + XP[i] >= M.front().size() ) { continue; }
		if (std::get<1>(Now) + YP[i] >= M.size()) { continue; }
		auto it = std::find_if(FP.begin(), FP.end(), [&](auto& o) {return (std::get<0>(o) == std::get<0>(Now) + XP[i]) && (std::get<1>(o) == std::get<1>(Now) + YP[i]); });

		if (it != FP.end()) { continue; }


		SearchPath_r(M, FP,{ std::get<0>(Now) + XP[i],std::get<1>(Now) + YP[i] }, Last, Wait + M[std::get<1>(Now) + YP[i]][std::get<0>(Now) + XP[i]], Min,FPR);
		


	}
	FP.pop_back();

	return true;



}

std::tuple<std::uintmax_t,FootPrint> SearchPath(const MapType& M, const Point& Fst, const Point& Last) {
	std::intmax_t Min = PoorSearch(M, Fst, Last);
	FootPrint fp;
	FootPrint FPR;
	//fp.push_back({ 0,0 });
	//ESS::Locate(std::get<0>(Fst)+Pa, std::get<1>(Fst)+Pa);
	//std::cout << '*';
	SearchPath_r(M, fp, Fst, Last, 0, Min,FPR);

	return { Min,FPR };

}

typedef std::tuple<Point, std::int64_t,std::uintmax_t,std::uintmax_t> FootData;//pos,dir,weight,now_weight
typedef std::vector<FootData> FootPrint2;

std::tuple<std::uintmax_t, FootPrint2> SearchPathL(const MapType& M, const Point& Fst, const Point& Last) {
	static std::array<int, 4> XP = { 0,1,0,-1 };
	static std::array<int, 4> YP = { 1,0,-1,0 };
	std::uintmax_t MinWeight = PoorSearch(M, Fst, Last);
	std::uintmax_t Weight = 0;

	FootPrint2 FP;
	FootPrint2 R;

	auto FW = M[std::get<1>(Fst)][std::get<0>(Fst)];
	FP.push_back({ Fst,0,FW,FW });

	while(FP.size()){
	//for (std::get<1>(FP.back()); std::get<1>(FP.back()) < 4; std::get<1>(FP.back())++) {
		auto i = std::get<1>(FP.back())++;
		if (i == 3) {
			FP.pop_back();
			continue;
		}
		auto& o = FP.back();
		auto& P = std::get<0>(o);
		Weight = std::get<3>(o);

		auto X = std::get<0>(P) + XP[i];
		auto Y = std::get<1>(P) + YP[i];

		if (X < 0) { continue; }
		if (Y < 0) { continue; }
		if (X >= M.front().size()) { continue; }
		if (Y >= M.size()) { continue; }

		auto it = std::find_if(FP.begin(), FP.end(), [&](auto& o) {return (std::get<0>(std::get<0>(o)) == X) && (std::get<1>(std::get<0>(o)) == Y); });
		if (it != FP.end()) { continue; }


		if (X == std::get<0>(Last) && Y == std::get<1>(Last)) {
			if (Weight <= MinWeight) {
				FP.push_back({ { X,Y }, i, M[Y][X], Weight + M[Y][X] });
				R = FP;
				MinWeight = Weight;

			}
			//FP.pop_back();
			continue;
		}
		else {

			if (Weight + M[Y][X] >= MinWeight) {
				if(i==3)FP.pop_back();
				continue;
			}
			else {
				Weight += M[Y][X];
			}
		}
		FP.push_back({ { X,Y },0, M[Y][X], Weight });
	}

	return { MinWeight,R };
}

MapType MakeVector() {
	MapType M{
		{0,2,9,5,3,9,4,1,3 },
		{7,1,5,4,6,7,9,8,8 },
		{8,3,4,1,1,2,9,4,6 },
		{2,3,7,1,6,5,4,2,6 },
		{4,7,3,8,5,7,3,6,0 },
	};

	return M;
}
MapType MakeVector2() {
	MapType M{
		{0,2,9,5,3,9,4,1,3,7,3},
		{7,1,5,4,6,7,9,8,8,1,4},
		{8,3,4,1,1,2,9,4,6,5,2},
		{2,3,7,1,6,5,4,2,6,8,4},
		{4,7,3,8,5,7,3,6,3,8,9},
		{5,5,2,6,3,7,4,2,9,6,1},
		{3,4,9,4,5,8,7,3,6,2,5},
		{9,6,7,5,5,4,2,1,8,6,7},
		{1,4,8,9,3,1,2,4,7,2,6},
		{7,1,9,1,1,4,7,8,5,2,1},
		{2,9,4,3,7,9,5,1,3,4,0},
	};

	return M;
}
int main() {
	//MapType M = MakeVector();
	MapType M2 = MakeVector2();
	FootPrint FP;
	FootPrint2 FP2;
	std::uintmax_t R;

	//std::tie(R,FP) = SearchPath(M, { 0,0 }, { 8,4 });
	//std::tie(R,FP2) = SearchPathL(M, { 0,0 }, { 8,4 });
	std::tie(R,FP2) = SearchPathL(M2, { 0,0 }, { 10,10 });
	std::cout << R << std::endl;

	for (auto& o : FP2) {
		//std::cout << std::get<0>(o) << ',' << std::get<1>(o) << ',' << M[std::get<1>(o)][std::get<0>(o)] << std::endl;
		std::cout << std::get<0>(std::get<0>(o)) << ',' << std::get<1>(std::get<0>(o)) << ',' << M2[std::get<1>(std::get<0>(o))][std::get<0>(std::get<0>(o))]<<','<< std::get<3>(o)<< std::endl;
	}

	return 0;
}