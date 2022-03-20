#include <stdio.h>
#include <stdlib.h>
#include <time.h>

double absVal(double number){
	return number > 0 ? number : -number;
}

// type = 1 for augmented matrix form
struct Matrix{
	double* mptr; // matrix pointer
	int row;
	int column;
	int type;
};

// 0 for user input values, 1 for random values
struct Matrix createMatrix(int random, int type);

// Matrix addition and multiplication
struct Matrix addMatrices(struct Matrix matrix1, struct Matrix matrix2);
struct Matrix multiplyMatrices(struct Matrix matrix1, struct Matrix matrix2);

void printMatrix(struct Matrix matrix);

// Fills the matrix with random values between 0 and up_to
void matrixRandomFill(struct Matrix matrix, int up_to);

// Taking input for matrix
void takeDimensions(int* row,int* column);
void takeValues(double* matrix, int row, int column);

// 3 basic operations
void changeRows(struct Matrix matrix, int row1, int row2);
void multiplyRow(struct Matrix matrix, int row, double multiplier);
void addRows(struct Matrix matrix, int row1, int row2, double multiplier);

// Finding Row Echelon Form (REF)
// and Reduced Row Echelon Form (RREF)
int findPivotAndSimplify(struct Matrix matrix, int pivot_count, int column);
void findRref(struct Matrix matrix); // argument must be REF matrix

// User actions
int takeAction();

void additionAction(int isRandom);
void multiplicationAction(int isRandom);
void refAction(int action_type, int isRandom); // 0 for ref, 1 for rref

int doAction(int command);


int main(){

	int cond = 1;

	while (cond){
		cond = doAction(takeAction());
	}


	return 0;

}

void printMatrix(struct Matrix matrix){
	printf("%c", '\n');
	for (int i = 0; i < matrix.row; i++){
		for (int j = 0; j < matrix.column; j++){
			if ((j == matrix.column - 1) && (matrix.type == 1)){
				printf("%s", " | ");
			}
			printf("%8.2lf", matrix.mptr[(matrix.column  * i) + j]);
		}
		printf("%c", '\n');
	}
	printf("%c", '\n');
}

void changeRows(struct Matrix matrix, int row1, int row2){
	printf("r%d <-> r%d\n", row1 + 1, row2 + 1);
	double temp;
	for (int j = 0; j < matrix.column; j++){
		temp = matrix.mptr[matrix.column * row1 + j];
		matrix.mptr[matrix.column * row1 + j] = matrix.mptr[matrix.column * row2 + j];
		matrix.mptr[matrix.column * row2 + j] = temp;
	}
	printMatrix(matrix);
}

void multiplyRow(struct Matrix matrix, int row, double multiplier){
	printf("r%d  -> r%d * %.2f\n",row + 1, row + 1, multiplier);
	for (int j = 0; j < matrix.column; j++){
		matrix.mptr[matrix.column * row + j] *= multiplier;
	}
	printMatrix(matrix);
}

void addRows(struct Matrix matrix, int row1, int row2, double multiplier){
	printf("r%d  -> r%d + r%d * %.2f\n", row1 + 1, row1 + 1, row2 + 1, multiplier);
	for (int j = 0; j < matrix.column; j++){
		matrix.mptr[matrix.column * row1 + j] += multiplier * matrix.mptr[matrix.column * row2 + j];
	}
	printMatrix(matrix);
}

int findPivotAndSimplify(struct Matrix matrix, int pivot_count, int column){
	int found_pivot = 0;
	for (int i = pivot_count; i < matrix.row; i++){
		if (absVal(matrix.mptr[matrix.column * i + column]) >= 0.01){
			found_pivot = 1;
			if (i != pivot_count){
				changeRows(matrix, pivot_count, i); 
			}
			if (matrix.mptr[matrix.column * pivot_count + column] != 1){
				multiplyRow(matrix, pivot_count, 1 / matrix.mptr[matrix.column * pivot_count + column]);
			}
			break;
		}
	}
	
	for (int i = pivot_count + 1; i < matrix.row; i++){
		if (matrix.mptr[matrix.column * i + column] != 0){
			addRows(matrix, i, pivot_count, -matrix.mptr[matrix.column * i + column]);
		}
	}
	return found_pivot;

}

void findRref(struct Matrix matrix){
	for (int i = matrix.row - 1; i >= 0; i--){
		int pivot_column;
		for (int j = 0; j < matrix.column; j++){
			if (matrix.mptr[matrix.column * i + j] == 1){
				pivot_column = j;
				break;
			}
		}
		for (int ii = i - 1; ii >= 0; ii--){
			if (matrix.mptr[matrix.column * ii + pivot_column] != 0){
				addRows(matrix, ii, i, -matrix.mptr[matrix.column * ii + pivot_column]);
			}
		}
	}
}

void takeDimensions(int* row,int* column){
	do{
		printf("%s", "Enter row and column: ");
		scanf("%d %d", row, column);
	} while ((*row <= 0) || (*column <= 0));
}

void takeValues(double* matrix, int row, int column){
	printf("%s", "Enter values:\n");
	for (int i = 0; i < row; i++){
        for (int j = 0; j < column; j++){
			scanf("%le", &matrix[(column  * i) + j]);
        }
	}
	printf("%c", '\n');
}

struct Matrix addMatrices(struct Matrix matrix1, struct Matrix matrix2){

	if ((matrix1.row != matrix2.row) || (matrix1.column != matrix2.column) ||  (matrix1.type != matrix2.type)){
		struct Matrix empty_matrix  = {NULL, 0, 0, 0};
		printf("%s", "Addition of these matrices is not possible.\n");
		return empty_matrix;
	}
	
	double* matrix_pointer = (double*) malloc(matrix1.row * matrix1.column * sizeof(double));
	for (int i = 0; i < matrix1.row; i++){
		for (int j = 0; j < matrix1.column; j++){
			matrix_pointer[matrix1.column*i + j] = matrix1.mptr[matrix1.column*i + j] + matrix2.mptr[matrix1.column*i + j];
		}
	}
	struct Matrix result_matrix = {matrix_pointer, matrix1.row, matrix1.column, matrix1.type};

	return result_matrix;
}

void matrixRandomFill(struct Matrix matrix, int up_to){
	for (int i = 0; i < matrix.row; i++){
		for (int j = 0; j < matrix.column; j++){
			matrix.mptr[(matrix.column  * i) + j] = rand() % up_to;
		}
	}
}

struct Matrix multiplyMatrices(struct Matrix matrix1, struct Matrix matrix2){

	if ((matrix1.column != matrix2.row) || (matrix1.type == 1) || (matrix2.type == 1)){
		struct Matrix empty_matrix = {NULL, 0, 0, 0};
		printf("%s", "Multiplication of these matrices is not possible.\n");
		return empty_matrix;
	}

	double* matrix_pointer = (double*) malloc(matrix1.row * matrix2.column * sizeof(double));

	for (int i = 0; i < matrix1.row; i++){
		for (int j = 0; j < matrix2.column; j++){
			double sum = 0;	
			for (int z = 0; z < matrix1.column; z++){
				sum += matrix1.mptr[i * matrix1.column + z] * matrix2.mptr[z * matrix2.column + j];
			}
			matrix_pointer[matrix2.column * i + j] = sum;
		}
	}

	struct Matrix result_matrix = {matrix_pointer, matrix1.row, matrix2.column, matrix1.type};

	return result_matrix;
}

struct Matrix createMatrix(int random, int type){
	int row, column;
	takeDimensions(&row, &column);

	double* matrixptr = (double*) malloc(row * column * sizeof(double));
	struct Matrix matrix = {matrixptr, row, column, type};
	if (!random){
		takeValues(matrixptr, row, column);
	}
	else if (random){
		int up_to;
		printf("%s", "Enter upper limit for values: ");
		scanf("%d", &up_to);
		matrixRandomFill(matrix, up_to);
	}

	return matrix;
}

void additionAction(int random){
	
	struct Matrix matrix_1 = createMatrix(random, 0);
	struct Matrix matrix_2 = createMatrix(random, 0);
	struct Matrix matrix_result = addMatrices(matrix_1, matrix_2);
	printMatrix(matrix_result);

	free(matrix_1.mptr);
	free(matrix_2.mptr);
	free(matrix_result.mptr);
}

void multiplicationAction(int random){
	
	struct Matrix matrix_1 = createMatrix(random, 0);
	struct Matrix matrix_2 = createMatrix(random, 0);

	struct Matrix matrix_result = multiplyMatrices(matrix_1, matrix_2); 
	printMatrix(matrix_result);

	free(matrix_1.mptr);
	free(matrix_2.mptr);
	free(matrix_result.mptr);
}

// User actions
int takeAction(){
	int command;
	printf("%s", "Enter command(0 for help): ");
	scanf("%d", &command);

	return command;
}

int doAction(int command){
	static int isRandom = 0;
	switch (command){
		case 0:
			printf("%s", "-1 for termination\n 0 for help\n 1 for addition\n"
					" 2 for multiplication\n 3 for row echelon form\n"
					" 4 for reduced ref\n 5 for random values for matrix\n\n");
			break;
		case -1:
			return 0;
		case 1:
			additionAction(isRandom);
			break;
		case 2:
			multiplicationAction(isRandom);
			break;
		case 3:
			refAction(0, isRandom);
			break;
		case 4:
			refAction(1, isRandom);
			break;
		case 5:
			isRandom = (isRandom + 1) % 2;
			break;
		default:
			printf("%s", "Invalid command\n");
			break;
	}
	return 1;
}

void refAction(int action_type, int random){
	struct Matrix matrix = createMatrix(random, 1);

	int pivot_count = 0;
	for (int j = 0; j < matrix.column - 1; j++){
		pivot_count += findPivotAndSimplify(matrix, pivot_count, j);
	}
	if (action_type == 1){
		findRref(matrix);
	}

	free(matrix.mptr);
}


