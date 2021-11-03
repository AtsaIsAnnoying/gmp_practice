//this is only for practice, it isn't a practical code, one main change could be optimzing the prime generation method.
//we take the size of the RSA key and the exponent as input and output the keys.
#include<stdio.h>
#include<gmp.h>
#include<stdlib.h>
#include<time.h>

//generation function 
void gene(mpz_t num,int k, int e,gmp_randstate_t generator)
{
 mpz_t num1,gcd_n,a;
 mpz_inits(gcd_n,num1,a,NULL);
 mpz_ui_pow_ui (a, 2,(k-1)/2 );
 do
 {
  mpz_urandomb (num, generator ,(k-1)/2 ); //generate an integer n of at most k bits
  mpz_add(num,num,a);
  mpz_sub_ui(num1,num,1);
  mpz_gcd_ui (gcd_n, num1,e);
 }while ((mpz_probab_prime_p ( num , 4 )==0)&&(mpz_cmp_ui(gcd_n,1)!=0));
 mpz_clears(num1,gcd_n,a,NULL);
}

int main( int argc, char* argv[])
{
//making sure the input is correct 
if( argc!=3)
{
 printf("usage of %s k,e\n",argv[0]);
 exit(-1);
}

//taking input the size in bits k and the exponent e.
int k = atoi( argv[1] ), e = atoi( argv[2] );

//declaring and initializing the variables
mpz_t n,p,q,phi_n,p1,q1,d,e_n;
mpz_inits(n,p,q,phi_n,p1,q1,d,e_n,NULL);

//declaring and initializing the generator
gmp_randstate_t generator ;
gmp_randinit_default(generator);
gmp_randseed_ui(generator, time(NULL)); 
//generating the primes p and q until n is k bits 
do
{
 gene(p,k,e,generator);
 gene(q,k,e,generator);
 mpz_mul(n,p,q);
}while(mpz_sizeinbase(n,2)!=k);
//calculating phi_n and d
mpz_sub_ui(p1,p,1);//p1=p-1
mpz_sub_ui(q1,q,1);//q1=q-1
mpz_mul(phi_n,p1,q1);//phi_n=(p1)*(q1)=(p-1)*(q-1).
mpz_set_ui(e_n,e);//e_n=e
mpz_invert (d, e_n , phi_n );// d= (e^-1) mod phi_n
gmp_printf("d=%Zx\n",d);
//opening the file to put the input inside
FILE *fp;
fp=fopen("public_keys.txt","w");
gmp_fprintf (fp,"n=%Zx\ne=%x\nd=%Zx\nphi=%Zx\n",n,e,d,phi_n);
fclose(fp);
//
mpz_clears(n,p,q,phi_n,p1,q1,d,e_n,NULL);
}
