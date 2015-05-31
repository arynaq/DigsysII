float f_overskudd[10];
float innflytting[10];

int main(int argc, char const *argv[])
{
	int sum = 0;
	int over_ten_K = 0;
	int increasing = 0;
	int decreasing = 0;

	for(int i=0; i<10; i++){
		sum += f_overskudd[i]-innflytting[i];

		if(innflytting[i] >= 10000)
			over_ten_K += 1;

		if(i<9 && f_overskudd[i+1]>f_overskudd[i]){
			decreasing = 0;
			increasing = 1;
		} 

		if(i<9 && f_overskudd[i+1]<f_overskudd[i]){
			decreasing = 1;
			increasing = 0;
		}
	}

	ïf(sum<0)
		printf("Nedgangen i befolkningsveksten er: %d", sum);
	else
		printf("Oppgangen i befolkningsveksten er: %d", sum);

	printf("Antall år med innflytting over 10k: %d", over_ten_K);

	if(increasing){
		if(decreasing)
			printf("Fødselsoverskuddet øker og avtar i tiårsperioden.");
		else
			printf("Fødselsoverskuddet øker i tiårsperioden");
	}
	if(decreasing){
		if(increasing)
			printf("Fødselsoverskuddet øker og avtar i tiårsperioden");
		else
			printf("Fødselsoverskuddet avtar i tiårsperioden");
	}

	return 0;
}