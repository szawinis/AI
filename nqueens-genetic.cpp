// Credits to https://arxiv.org/pdf/1802.02006.pdf
#include <bits/stdc++.h>
using namespace std;
const int N = 50;
const int initialPopulationCount = 150, eliteBucket = 13;

struct Individual {
    array<int, N> arrangement;
    int cost;
    void recompute_cost() {
        cost = 0;
        for(int i = 0; i < N; i++) {
            for(int j = i + 1; j < N; j++) {
                assert(arrangement[i] != arrangement[j]);
                if(i - arrangement[i] == j - arrangement[j] || i + arrangement[i] == j + arrangement[j]) ++cost;
            }
        }
    }
    Individual() {
        for(int i = 0; i < N; i++) arrangement[i] = i;
        random_shuffle(arrangement.begin(), arrangement.end());
        recompute_cost();
    }
};

Individual* crossover(Individual* x, Individual* y) {
    Individual* ret = new Individual();

    int pos1 = rand() % (N - 1);
    int pos2 = pos1 + 1 + rand() % (N - pos1 - 1);
    assert(0 <= pos1 && pos1 < pos2 && pos2 < N);

    set<int> in_range;
    for(int i = pos1; i <= pos2; i++) in_range.insert(x->arrangement[i]);
    vector<int> not_in_range;
    for(int i = 0; i < N; i++) if(!in_range.count(y->arrangement[i]))
        not_in_range.push_back(y->arrangement[i]);

    for(int i = pos1; i <= pos2; i++) ret->arrangement[i] = x->arrangement[i];
    int curr = 0;
    for(int i = 0; i < pos1; i++) ret->arrangement[i] = not_in_range[curr++];
    for(int i = pos2+1; i < N; i++) ret->arrangement[i] = not_in_range[curr++];
    ret->recompute_cost();
    return ret;
}

void mutate(Individual* x) {
    int pos1 = rand() % N;
    int pos2 = rand() % N;
    swap(x->arrangement[pos1], x->arrangement[pos2]);
    x->recompute_cost();
}

Individual* GA() {
    vector<Individual *> population;
    for(int i = 0; i < initialPopulationCount; i++) population.push_back(new Individual());
    int cnt = 0;
    while(true) {
        ++cnt;
        vector<Individual *> newPopulation;
        for(int i = 0; i < population.size(); i++) {

            // elitist -> only reproduce in the elite cohort
            sort(population.begin(), population.end(), [] (auto *a, auto *b) { return a->cost < b->cost; }); // lower cost means better
            int x = rand() % population.size() % eliteBucket;
            int y = rand() % population.size() % eliteBucket;

            // all individuals in next generation are produced from crossover - none get to stay
            Individual *tmp = crossover(population[x], population[y]);
            if(rand() % 5) mutate(tmp); // 0.8 probability of mutation
            if(tmp->cost == 0) {
                cout << "Generations: " << cnt << endl;
                return tmp;
            }
            newPopulation.push_back(tmp);

        }
        for(int i = 0; i < population.size(); i++) delete population[i];
        population = newPopulation;
    }
}

int main() {
    srand(time(NULL));
    Individual* ans = GA();
    for(int i = 0; i < N; i++) cout << ans->arrangement[i] << ' ';
}
