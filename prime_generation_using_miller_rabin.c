#include<stdio.h>
#include<gmp.h>
#include<stdlib.h>
#include<time.h>


//a fast way to calculate res=base^exp mod mod
void square_and_multiply(mpz_t res, mpz_t base, mpz_t exp, mpz_t mod)
{
mpz_set_ui(res,1);
int k=mpz_sizeinbase(exp,2);
for(int i=k;i>=0;i--)
{
 mpz_powm_ui (res, res , 2, mod);
 if (mpz_tstbit (exp,i))
 {
  mpz_mul (res, res, base);
  mpz_mod (res, res, mod);
 }
}
}
//writing n-1 as 2^s.r 
void form(mpz_t n, mpz_t r, mpz_t s)
 {
  //n1=n-1
  mpz_t n1,n2;
  mpz_inits(n1,n2,NULL);
  mpz_sub_ui(n1,n,1);
  mpz_set(n2,n1);//n2=n1
  //defining s
  int sx=0;
  while(mpz_divisible_ui_p (n1, 2))
   {
    mpz_cdiv_q_ui(n1, n1,2 );
    
    sx++;
   }
   mpz_set_ui(s,sx);
   //defining r
   mpz_ui_pow_ui (r, 2 , sx );
   mpz_cdiv_q (r, n2, r);
   mpz_clears(n1,n2,NULL);
  }
   
//testing the probable primality of n according to the base a  using miller rabin test 
int test_miller_rabin_base(mpz_t n, mpz_t a)
 {
  int j,res=0; 
  mpz_t y,n1,s1,r,s;
  mpz_inits(y,n1,s1,r,s,NULL);
  
  
  form(n, r, s);//writing n-1 as 2^s.r 
  mpz_sub_ui(n1,n,1);//n1=n-1
  mpz_sub_ui(s1,s,1);//s1=s-1
  square_and_multiply(y, a, r,n);//y=a^r mod n
  
  //the test 
  if((mpz_cmp_ui(y,1)!=0)&&(mpz_cmp(y,n1)!=0))
   {
    j=1;
    while((mpz_cmp_ui(s1,j)>=0)&&(mpz_cmp(y,n1)!=0))
     {
      mpz_powm_ui(y,y,2,n);
      if(mpz_cmp_ui(y,1)==0)
       {
        res=1;
       }
      j++;
     }
    if(mpz_cmp(y,n1)!=0)
     {
      res=1;
     }
    }
   //clearing the excess
  mpz_clears(y,n1,s1,r,s,NULL);
  return res;
 }

int test_miller_rabin(mpz_t n, int t, gmp_randstate_t generator)
 {
  int res;
  mpz_t a,n1;
  mpz_inits(a,n1,NULL);
  mpz_sub_ui(n1,n,1);
  for(int i=1;i<=t;i++)
   {
    mpz_urandomm (a,generator, n1);
    if(mpz_cmp_ui(a,2)<0)
     {
      mpz_add_ui(a,a,2);
     }
    res=test_miller_rabin_base(n,a);
   }
   return res;
  }
   
  

//main function that takes as input the number n , the base a and determine if n is probably prime or composite
int main(int argc, char *argv[])
{

//making sure we have the correct input
 if(argc!=4)
  {
   printf("Using %s: b t r\n",argv[0]);
   exit(-1);
  }
  
//taking the input 
int b=atoi(argv[1]),t=atoi(argv[2]),r=atoi(argv[3]),res,counter=0,rs;

//a=2^b-1
mpz_t a,z_n[r];
mpz_init(a);
mpz_ui_pow_ui (a, 2, b-1);

//the random generator 
gmp_randstate_t generator ;//declaring a generator
gmp_randinit_default(generator); // Initializing the generator
gmp_randseed_ui(generator, time(NULL)); // giving the generator a seed

//dec+in
for(int i=0;i<r;i++)
 {
  mpz_t z_n[i];
 }
for(int i=0;i<r;i++)
 {
   mpz_init(z_n[i]);
 }
while(counter<r)
 { 
  mpz_urandomb (z_n[counter], generator ,b-1 ); 
  mpz_add(z_n[counter],z_n[counter],a);
  //applying the test
  res=test_miller_rabin(z_n[counter],t,generator);
  //counting the probable primes that are generated
  if(res==0)
   {
    rs=mpz_probab_prime_p(z_n[counter], 2 );
    
    if(rs==0)
     {
      gmp_printf("%Zd is a flase prime\n",z_n[counter]);
     }
    counter++;
   }
   
 }
for(int i=0;i<r;i++)
 {
  mpz_clear(z_n[i]);
 }
gmp_randclear (generator);//clearing the generator
mpz_clear(a);
return 0;
} 
