#include<stdio.h>
#include<gmp.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include <sys/types.h>
#include <sys/resource.h>

//prime tester, returns 0 if prime and 1 otherwise 
int IsPrime(int x)
 { 
  int res=0;
  for(int i=2;i<=sqrt(x);i++)
   {
    if(x%i==0)
     {
      res=1;
      break;
     }
   }
   return res;
  }

//this function generates k primes strating from 2
void prime_generation( int arr[], int k)
 {
  arr[0]=2;
  int counter=1,i=3;
  while(counter<k)
   {
    if(IsPrime(i)==0)
     {
      arr[counter]=i;
      counter++;
     }
    i++;
  }
 }
 
//testing the divisibilty by all the items in the array, return 1 if n is divisble by anyone of the k primes 
int test_with_array(mpz_t z_n[],mpz_t n,int k,int arr[])
 {
  int res=0;
  for(int i=0;i<k;i++)
   {
    mpz_mod_ui(z_n[i],n,arr[i]);
    if(mpz_cmp_ui(z_n[i],0)==0)
     {
      res=1;
      break;
     }
   }
  return res;
 }

//adding 2
void addingtwo(mpz_t z_n[],mpz_t n, int k, int arr[])
 {
   for(int i=0;i<k;i++)
    {
     mpz_add_ui(z_n[i],z_n[i],2);
     mpz_mod_ui(z_n[i],z_n[i],arr[i]);
    }
   mpz_add_ui(n,n,2);
 }
//the miller rabin test 
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
//the optimized sieve function
void optimized_sieve(mpz_t q, int k, gmp_randstate_t generator, int arr[], int b, int t)
 {
  mpz_t a,z_n[k]; //declaring the variable n
  mpz_init(a); //initializing the variable n
  for(int i=0;i<k;i++)
   {
    mpz_t z_n[i];
   }
  for(int i=0;i<k;i++)
   {
    mpz_init(z_n[i]);
   }
  
  mpz_ui_pow_ui (a, 2, b-1);
  do
  {
   mpz_urandomb (q, generator ,b-1 ); //generate an integer n of at most k bits
   mpz_add(q,q,a);
  }while(mpz_even_p (q ));
  prime_generation(arr,k);//generate an array of k primes 
  while(test_with_array(z_n,q,k,arr)!=0)
   {
    addingtwo(z_n,q,k,arr);
   }
  while(test_miller_rabin(q,t,generator)!=0)
   {
    addingtwo(z_n,q,k,arr);
   }
   
   mpz_clear(a);
   for(int i=0;i<k;i++)
   {
    mpz_clear(z_n[i]);
   }
 }
 
unsigned long int cputime()
{
  struct rusage rus;

  getrusage (0, &rus);
  return rus.ru_utime.tv_sec * 1000 + rus.ru_utime.tv_usec / 1000;
}
 
int main(int argc, char *argv[])
 {
  if (argc!=5)
   {
    printf("Using %s: b kmax t r\n",argv[0]);
    exit(-1);
   }
  int b=atoi(argv[1]),kmax=atoi(argv[2]),t=atoi(argv[3]),r=atoi(argv[4]);
  unsigned long int start;
  
  mpz_t q,z_n[r];
  mpz_init(q);
  
  //the random generator 
  gmp_randstate_t generator ;//declaring a generator
  gmp_randinit_default(generator); // Initializing the generator
  gmp_randseed_ui(generator, time(NULL)); // giving the generator a seed   
  
  //the result
  //dec+in
for(int i=0;i<r;i++)
 {
  mpz_t z_n[i];
 }
for(int i=0;i<r;i++)
 {
   mpz_init(z_n[i]);
 }
 
//creating the file that will contain all the input 
FILE *fp;
fp=fopen("timing2.txt","w+");
for(int j=1;j<=kmax;j++)
 {
 printf("for j=%d\n",j);
 int arr[j];
 start = cputime();
  for(int i=0;i<r;i++)
   { 
    optimized_sieve(z_n[i],j, generator, arr, b, t);
   }
  fprintf(fp,"%d %lu\n",j,cputime() - start);
 }
fclose(fp);
for(int i=0;i<r;i++)
 {
  mpz_clear(z_n[i]);
 }
  
  gmp_randclear (generator);//clearing the generator
  mpz_clear(q);
  return 0;
  }
