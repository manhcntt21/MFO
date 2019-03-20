#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<time.h>
#include<algorithm>
#include<iostream>
struct TSP {
    float value ;
    int index;
};
typedef struct TSP TSP ;
struct TSP1 {
    float P_mu ;
    float R_mu;
};
typedef struct TSP1 TSP1 ;
int number_Population = 50;
int number_Population1 = number_Population;
int number_Population2 = number_Population;
int number_current_population = number_Population;
int rmp = 0.3; // random mating probability
float **P;
int **Q;
int **R;
int **distance;
// số đỉnh của task1 và task2
int number_vertex1 = 15;
int number_vertex2 = 20;
int m = (number_vertex1 > number_vertex2) ? number_vertex1 : number_vertex2;   //printf("m = %d",m);
int *weight ;    // khợi tạo trong khoảng từ 1 đến 20
int *value ;     // khởi tạo giá trị của đồ vật từ 1 đến 50
int *factory_cost;
int *factory_rank;
int W_MAX = 50 ; // trọng lượng của cái tùi
int min_cost_task1;
int max_cost_task1;
int min_cost_task2;
int max_cost_task2;
float *_scalar_fitness;
// hàm khởi tạo quần thể
void initiation_Population();
void evaluate_individuals_task1(int nnum,int num);
void factory_cost_task1(int num);
void factory_rank_task1(int num);
void sort_task1(int *&a,int n);
void crossover_task(int i,int j);
void swap_crossver_task1_float(float *&a,float *&b);
void swap_crossver_task1_integer(int* &a, int* &b);
void swap_crossver_value(float *a, float *b);
void mutation_task(int tmp);
void evaluate_individuals_task2(int nnum,int num);
void sort_task2(TSP *a);
void factory_cost_task2();
int *_factory_cost;
int *_factory_rank;
int *skill_factor; //  ca the P giải tốt bài nào
void factory_rank_task2();
int _fitness_task;
void _sort_task2(int *&a,int n);
void crossover_task2(int i, int j);
void mutation_task2(int tmp);
void swap__task2(int *a, int *b);
void assorttative_mating();
void evaluate_individuals();
void evaluate_individuals_P_comma();
void _skill_factor(int num);
void scalar_fitness();
void selection();
int main()
{
    SetConsoleOutputCP(65001) ;
    P = new float*[number_Population*2] ;
    Q = new int*[number_Population*2] ;
    R = new int*[number_Population*2] ;
    distance = new int *[number_vertex2];
    for(int i = 0; i < number_Population*2 ; i++)  // cấp phát bộ nhớ cho mỗi cá thể
        P[i] = new float[m];
    for(int i = 0; i < 2*number_Population ; i++)  // cấp phát bộ nhớ cho mỗi cá thể
        Q[i] = new int[number_vertex1];
    for(int i = 0; i < 2*number_Population ; i++)  // cấp phát bộ nhớ cho mỗi cá thể
        R[i] = new int[number_vertex2];
    for(int i = 0 ; i < number_vertex2 ; i++) {
        distance[i] = new int [number_vertex2];
    }
// khởi tạo trọng lượng và giá trị của mỗi đồ vật
    weight = new int[number_vertex1];
    value = new int[number_vertex1];
    factory_cost = new int[number_Population*2];
    factory_rank = new int[number_Population*2];
    _factory_cost = new int[number_Population*2];
    _factory_rank = new int[number_Population*2];
    skill_factor = new int[number_Population*2];
    _scalar_fitness = new float[number_Population*2];
    srand(time(0));
    for( int i = 0 ; i < number_vertex1 ; i++)
        weight[i] = rand()%20 + 1;
    printf("Weight:     ");
    for( int i = 0 ; i < number_vertex1 ; i++)
        printf("%d ",weight[i]);
    for( int i = 0 ; i < number_vertex1 ; i++)
        value[i] = rand()%50 + 1 ;
    printf("\nValue:      ");
    for( int i = 0 ; i < number_vertex1 ; i++)
        printf("%d ",value[i]);
    printf("\nMang distance: \n");
    for(int i = 0; i < number_vertex2;i++) {
        for(int j = i+1 ; j < number_vertex2;j++) {
            distance[i][j] = distance[j][i] = rand()%50+1;
        }
    }
    for(int i = 0 ; i < number_vertex2;i++)
        distance[i][i] = 0;
    for(int i = 0; i < number_vertex2;i++) {
        for(int j = 0 ; j < number_vertex2;j++) {
            printf("%-5d ",distance[i][j]);
        }
        printf("\n");
    }
// Khởi tạo quần thể
    initiation_Population();
// đánh giá mỗi cá thể dựa trên task tương ưng:
    for(int i = 0 ; i < 2 ; i++) {
        evaluate_individuals();
        // bắt đầu vòng lập ở đây
        assorttative_mating();
        evaluate_individuals_P_comma();
        scalar_fitness();
        selection();
    }
    printf("\n\n\nArray P:-----------------------------------\n");
    for(int i = 0 ; i < number_current_population ; i ++) {
        for(int j = 0; j < m ; j++)
            printf("%.2f   ",P[i][j]);
        printf("\n");
    }

    printf("\n\n\nArray Q:-----------------------------------\n");
    for(int i = 0 ; i < number_current_population ; i ++) {
        for(int j = 0; j < number_vertex1 ; j++)
            printf("%d   ",Q[i][j]);
        printf("\n");
    }

    printf("\n\n\nArray P:-----------------------------------\n");
    for(int i = 0 ; i < number_current_population ; i ++) {
        for(int j = 0; j < number_vertex2 ; j++)
            printf("%d   ",R[i][j]);
        printf("\n");
    }
    // hủy bộ nhớ cấp phát động
    for(int i = 0 ; i < number_Population*2 ; i++)
        delete [] P[i];
    for(int i = 0 ; i < number_Population*2 ; i++)
        delete [] Q[i];
    for(int i = 0 ; i < number_Population*2 ; i++)
        delete [] R[i];
    for(int i = 0 ; i < number_vertex2 ; i++)
        delete [] distance[i];
    delete [] P ;
    delete [] Q;
    delete [] R;
    delete [] distance;
    delete weight;
    delete value;
    delete [] factory_cost;
    delete [] factory_rank;
    delete [] _factory_cost;
    delete [] _factory_rank;
    delete [] _scalar_fitness;
    return 0;
}
// mảng P là mảng khởi tạo bạn đầu(các số thực ngẫu nhiên trong khoảng 0,1 ).
void initiation_Population()   {
    srand(time(0));
    for(int i = 0 ; i < number_Population ; i ++)
        for(int j = 0; j < m ; j++)
            P[i][j] = ((rand()%100)*1.0/100);
    printf("\n");
    printf("\nArray P:------------------------------------------\n");
    for(int i = 0 ; i < number_current_population ; i ++) {
        for(int j = 0; j < m ; j++)
            printf("%.2f   ",P[i][j]);
        printf("\n");
    }
}
// chuyển P sang Q , với Q là các giá trị 0 hoặc 1.
void evaluate_individuals_task1(int nuum,int num) {
    float W ;
    int mark[num] = {0};
    for(int i = nuum ; i < num ; i++) {
            W = 0.0;
        for (int j = 0 ; j < number_vertex1 ; j++)
        {
            if( P[i][j] < 0.5 )
                Q[i][j] = 0;
            else Q[i][j] = 1;
        }
// tính xem nó có thõa mãn trong lượng nhỏ hơn khối lượng không, nếu không thì khởi tạo lại Q
        for(int j = 0 ; j < number_vertex1;j++) {
                    if(Q[i][j] == 1) {
                        W+= weight[j];
                    }
                }
        if(W > W_MAX) mark[i] = 0;
        else mark[i] = 1;
// nếu cá thể từ Q không thõa mãn thì sinh lại từ P, và kiếm tra Q từ P đấy, đến bao giờ được thì thôi
        while(mark[i] == 0) {
            for(int j = 0 ; j < m ; j++) {
                P[i][j] = ((rand()%100)*1.0/100)  ;
            }
            for (int j = 0 ; j < number_vertex1 ; j++) {
                if( P[i][j] < 0.5 ) Q[i][j] = 0;
                else Q[i][j] = 1;
            }
            W = 0.0;
            for(int j = 0 ; j < number_vertex1;j++) {
                if(Q[i][j] == 1) {
                    W+= weight[j];
                }
            }
            if(W > W_MAX) mark[i] = 0;
            else mark[i] = 1;
        }
    }
    printf("\nArray Q:------------------------------------------------\n");
    for(int i = 0 ; i < number_current_population ; i ++)
    {
        for(int j = 0; j < number_vertex1 ; j++)
            printf("%d   ",Q[i][j]);
        printf("\n");
    }
}
// bài toán cái túi: tính toán giá trị của các lời giải
void factory_cost_task1(int num) {
    float sum_value;
    for(int i = 0 ; i < num ;i++)
    {
        sum_value = 0.0;
        for (int j = 0; j < number_vertex1 ; j++)
        {
            if(Q[i][j] == 1)
                sum_value += value[j];
        }
        factory_cost[i] = sum_value;
    }
    for(int i = 0 ; i < num;i++) {
        printf("%d ",factory_cost[i]);
    }
    printf("\n");
}
// tính rank(sắp xếp) lưu ra một mảng mới và fitness
void factory_rank_task1(int num) {
    int *tmp = new int[num];
    int mark[num] = {0};
    for( int i = 0; i < num;i++)
        tmp[i] = factory_cost[i];
    sort_task1(tmp,num);
    for(int i = 0; i < num; i++) {
        for(int j = 0; j < num; j++) {
            if((factory_cost[i]==tmp[j])&&(mark[j]==0)) {
                factory_rank[i] = j + 1;
                mark[j] = 1;
                break;
            }
            else if((tmp[i]==factory_cost[j])&&(mark[j]!=0))
            {
               continue;
            }
        }
    }
//    fitness_task1 = factory_rank[0];
    for(int i = 0 ; i < num;i++) {
        printf("%d ",factory_rank[i]);
    }
    printf("\n");

    free(tmp);
}
//                   hàm sắp xếp từ lớn đến bé
void sort_task1(int* &a, int n) {
    int tmp;
    for( int i = 0; i < n - 1 ; i++ )
    {
        for(int j = i + 1; j < n ; j++)
                if(a[i] < a[j] ) {
                    tmp = a[i];
                    a[i] = a[j];
                    a[j] = tmp;
                }
    }
    min_cost_task1 = a[n-1];
    max_cost_task1 = a[0];
}
//                   lai ghép điểm: nếu 2 con mới tốt hơn bố mẹ thì thay thế, ngước lại bỏ qua
// vể xem lại.

//Simple arithmetic crossover
void crossover_task(int i, int j) {
    int random;
    float anpha = 0.5;
    random = rand()%m;
    float *tmp1=(float*)malloc(m*sizeof(float));
    float *tmp2=(float*)malloc(m*sizeof(float));

    for(int k = 0 ; k < random;k++) {
        *(tmp1+k) = P[i][k];
        *(tmp2+k) = P[j][k];
    }
    for(int k = random ; k < m;k++) {
        *(tmp1+k) = anpha*P[j][k] + (1-anpha)*P[i][k];
        *(tmp2+k) = anpha*P[i][k] + (1-anpha)*P[j][k];
    }
    for(int k = 0 ; k < m ;k++) {
        P[number_current_population][k] = tmp1[k];
    }
    number_current_population++;
    for(int k = 0 ; k < m ;k++) {
        P[number_current_population][k] = tmp1[k];
    }
    number_current_population++;
    free(tmp1);
    free(tmp2);
}
void swap_crossver_task1_float(float* &a, float* &b) {
   float *c = a;
          a = b;
          b = c;
}
void swap_crossver_task1_integer(int* &a, int* &b) {
   int *c = a;
          a = b;
          b = c;
}
//                   đột biến điểm: chọn ngẫu nhiễn một cá thể
//                                  chọ ngẫu nhiên một điểm đột biến
//                                  nếu cá thể đột biến tốt hơn thì thay thế ngược lại bỏ qua
void mutation_task(int i) {
    int random ;
    int number_random;
    float *tmp = (float*)malloc(m*(sizeof(float)));
    int first_mutation_point;
    int second_mutation_point;
    int array_tmp[m];
    for(int k = 0 ; k < m ; k++) {
        array_tmp[k] = k;
    }
    std::random_shuffle(&array_tmp[0],&array_tmp[m]);
    first_mutation_point = array_tmp[0];
    second_mutation_point = array_tmp[1];
    for(int k = 0; k < m;k++) {
        *(tmp+k) = P[i][k];
    }
    // dùng tạm hàm này để swap
    swap_crossver_value(&tmp[first_mutation_point],&tmp[second_mutation_point]);
    for(int k = 0; k < m;k++) {
        P[number_current_population][k] = *(tmp+k);
    }
    number_current_population++;
    free(tmp);
}
// swap_crossever_value
void swap_crossver_value(float *a, float *b) {
    float c = *a;
    *a = *b;
    *b = c;
}
// bài toán người du lịch:
void evaluate_individuals_task2(int nnum,int num) {
    TSP **abc=(TSP**)malloc(num*sizeof(TSP*));
    for(int i = nnum ; i < num ; i++) {
        abc[i] =(TSP*)malloc(number_vertex2*sizeof(TSP));
    }
// gán giá trị cho mảng a:
    for(int i1 = nnum ; i1 < num ; i1++) {
        for(int j = 0 ; j < number_vertex2 ; j++) {
            abc[i1][j].value = P[i1][j];
            abc[i1][j].index = j + 1;
        }
        sort_task2(abc[i1]);
    }

    for(int i = nnum ; i < num ; i++) {
        for(int j = 0 ; j < number_vertex2 ; j++) {
                R[i][j] = abc[i][j].index ;
        }
    }
    printf("\nArray R:----------------------------------------\n");
    for(int i = 0 ; i < num ; i++) {
        for(int j = 0 ; j < number_vertex2; j++)
            printf("%d ",R[i][j]);
      printf("\n");
    }

    for(int i = nnum ; i < num ; i++)
        free(abc[i]);
    free(abc);
}
void sort_task2(TSP *a) {
    TSP tmp;
    for(int j = 0 ; j < number_vertex2 -1 ; j++) {
        for(int k = j + 1 ; k < number_vertex2 ; k++) {
            if( a[j].value < a[k].value) {
                tmp  = a[j];
                a[j]= a[k];
                a[k] =  tmp;
            }
        }
    }
}
void factory_cost_task2(int num) {
    int c;
    int d;
    for(int i = 0 ; i < num ; i++) {
            _factory_cost[i] = 0;
        for(int j = 0 ; j < number_vertex2 -1 ; j ++) {
            c = (int)R[i][j] - 1;
            d = (int)R[i][j+1] - 1;
            _factory_cost[i] += distance[c][d];
        }
        c = (int)R[i][number_vertex2-1]-1;
        d = (int)R[i][0]-1;
        //printf("%d , %d\n",c,d);
        _factory_cost[i]+= distance[c][d];
    }
    for(int i = 0 ; i < num ; i++) {
        printf("%d ",_factory_cost[i]);

    }
}
void factory_rank_task2(int num) {
    int *tmp = new int[num];
    int mark[num] = {0};
    for( int i = 0; i < num;i++)
        tmp[i] = _factory_cost[i];
    _sort_task2(tmp,num);
    for(int i = 0; i < num; i++) {
        for(int j = 0; j < num; j++) {
            if((_factory_cost[i] == tmp[j])&&(mark[j]==0)) {
                _factory_rank[i] = j + 1;
                mark[j] = 1;
                break;
            }
            else if((tmp[i]==_factory_cost[j])&&(mark[j]!=0))
            {
               continue;
            }
        }
    }
    _fitness_task = _factory_rank[0];
    for(int i = 0 ; i < num;i++) {
        printf("%d ",_factory_rank[i]);
    }
    printf("\n");

    free(tmp);
}
void _sort_task2(int* &a, int n) {
    int tmp;
    for( int i = 0; i < n - 1 ; i++ )
    {
        for(int j = i + 1; j < n ; j++)
                if(a[i] > a[j] ) {
                    tmp = a[i];
                    a[i] = a[j];
                    a[j] = tmp;
                }
    }
    max_cost_task2 = a[0];
    min_cost_task2 = a[n-1];
}
void swap__task2(int *a, int *b) {
    int tmp = *a;
        *a = *b;
        *b = tmp;
}
void evaluate_individuals() {
    evaluate_individuals_task1(0,number_Population);
    evaluate_individuals_task2(0,number_Population);
// tính hệ số kỹ năng
// + giá trị của các đồ vật::
    printf("\nfactory cost ban dau task1------------------------------------- \n");
    factory_cost_task1(number_Population);
// + sắp xếp rank và tính  + scalar fitness
    printf("\nfactory rank ban dau task1-----------------------------------------\n");
    factory_rank_task1(number_Population);

    printf("\nfactory cost ban dau task2----------------------------------------\n");
    factory_cost_task2(number_Population);
    printf("\nfactory rank ban dau task2----------------------------------------\n");
    factory_rank_task2(number_Population);
    printf("\nSkill factor ---------------------------------------------------\n");
    _skill_factor(number_Population);

}
void assorttative_mating() {
    float _rand = (rand()%100)*1.0/100;
    int __rand;
    int tmp1;
    int tmp2;
    int array_tmp[number_Population];
    for(int i = 0 ; i < number_Population ; i++) {
        array_tmp[i] = i;
    }
    for(int i = 0 ; i < number_Population ; i = i + 2) {
        std::random_shuffle(&array_tmp[0],&array_tmp[number_Population]);
        tmp1 = array_tmp[0];
        tmp2 = array_tmp[1];
        if (skill_factor[tmp1] == skill_factor[tmp2] || _rand < rmp) {
            crossover_task(tmp1,tmp2);
            if( skill_factor[tmp1] != skill_factor[tmp2]) {
                if(_rand < 0.5) {
                    skill_factor[number_Population+i] = 1;
                    skill_factor[number_Population+i+1] = 1;
                }
                else{
                    skill_factor[number_Population+i] = 2;
                    skill_factor[number_Population+i] = 2;
                }
            } else {
                skill_factor[number_Population+i] = skill_factor[number_Population+i+1] = skill_factor[tmp1];
            }
        } else{
            mutation_task(tmp1);
            skill_factor[number_Population+i] = skill_factor[tmp1];
            mutation_task(tmp2);
            skill_factor[number_Population+i+1] = skill_factor[tmp2];
        }
        printf("\n(%d %d)\n", skill_factor[tmp1], skill_factor[tmp2]);
    }
    printf("number_current_population = %d\n\n\n",number_current_population);
}
void evaluate_individuals_P_comma() {
    evaluate_individuals_task1(number_Population,number_current_population);
    evaluate_individuals_task2(number_Population,number_current_population);
    printf("\nCost 2 task2 khi chua update\n");
    factory_cost_task1(number_current_population);
    factory_cost_task2(number_current_population);
    for(int i = number_Population; i < number_current_population ;i++) {
        if(skill_factor[i] == 1) _factory_cost[i] = 1000;
        else if(skill_factor[i] == 2) factory_cost[i] = 0;
    }
    printf("\nCost 2 task2 khi da update\n");
    printf("\nfactory_cost 1 ---------------------------------------------------\n");
    for(int i = 0; i < number_current_population;i++) {
        printf("%d ",factory_cost[i]);
    }
    printf("\nfactory_cost 2 ---------------------------------------------------\n");
    for(int i = 0; i < number_current_population;i++) {
        printf("%d ",_factory_cost[i]);
    }
    printf("\nfactory rank task1--------------------------------------------\n");
    factory_rank_task1(number_current_population);
    printf("\nfactory rank task2--------------------------------------------\n");
    factory_rank_task2(number_current_population);
    printf("\nSkill factor ---------------------------------------------------\n");
    for(int i = 0; i < number_current_population;i++) {
        printf("%d ",skill_factor[i]);
    }
}
void _skill_factor(int num) {
    for(int i = 0; i < num;i++) {
        if(factory_rank[i] > _factory_rank[i]) {
            skill_factor[i] = 2;
        } else if (factory_rank[i] < _factory_rank[i]) {
            skill_factor[i] = 1;
        } else {
            skill_factor[i] = rand()%2 + 1;
        }
    }
    for(int i = 0 ; i < num;i++) {
        printf("%d ",skill_factor[i]);
    }
    printf("\n");
}
void scalar_fitness() {
    // cách tính co chút ngươc nhau
    // do cái túi thì cần cost lớn, còn TSP cần cost nhỏ
    for(int k = 0 ; k < number_current_population; k++) {
        if(skill_factor[k] == 1) _scalar_fitness[k] = (factory_cost[k] - min_cost_task1)*1.0/(max_cost_task1 - min_cost_task1);
        else if(skill_factor[k] == 2 )_scalar_fitness[k] = (_factory_cost[k] - min_cost_task2)*1.0/(max_cost_task2 - min_cost_task2);
    }
    printf("\n\n");
    for(int k = 0 ; k < number_current_population; k++) {
        printf("%.2f ",_scalar_fitness[k]);
    }
}
void selection() {
    printf("\nscalar fitness: \n");
    for(int k = 0; k < number_current_population; k++) {
        printf("%.2f " ,_scalar_fitness[k]);
    }
    // sort _scalar_fitness để chọn cá thế tốt: giảm dần
    // _scalar_fitness càng cao càng tốt.
    for(int i = 0 ; i < number_current_population - 1 ; i++) {
        for(int j = i + 1 ; j  < number_current_population ; j++) {
            if(_scalar_fitness[i] < _scalar_fitness[j]) {

                swap_crossver_value(&_scalar_fitness[i],&_scalar_fitness[j]);
                swap_crossver_task1_float(P[i],P[j]);
                swap_crossver_task1_integer(Q[i],Q[j]);
                swap_crossver_task1_integer(R[i],R[j]);
            }
        }
    }
    number_current_population = number_Population;
}
