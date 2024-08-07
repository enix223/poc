#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

static int x, y, r1, r2;
static int r1v[2] = {0, 0}, r2v[2] = {0, 0};

int iteration(void);

void *thread1(void *args)
{
  x = 1;
  r1 = y;
  pthread_exit(NULL);
}

void *thread2(void *args)
{
  y = 2;
  r2 = x;
  pthread_exit(NULL);
}

int iteration()
{
  int rc;
  pthread_t t1, t2;

  x = 0;
  y = 0;
  r1 = 0;
  r2 = 0;

  /* create thread1 */
  rc = pthread_create(&t1, NULL, thread1, NULL);
  if (rc)
  {
    fprintf(stderr, "error: pthread_create, rc: %d\n", rc);
    return EXIT_FAILURE;
  }

  /* create thread2 */
  rc = pthread_create(&t2, NULL, thread2, NULL);
  if (rc)
  {
    fprintf(stderr, "error: pthread_create, rc: %d\n", rc);
    return EXIT_FAILURE;
  }

  /* wait for thread1 & thread2 to end */
  pthread_join(t1, NULL);
  pthread_join(t2, NULL);

  r1v[r1 == 0 ? 0 : 1] += 1;
  r2v[r2 == 0 ? 0 : 1] += 1;

  return EXIT_SUCCESS;
}

int main(int argc, char const *argv[])
{
  int rc, i;

  for (i = 0; i < 10000; i++)
  {
    if (rc = iteration())
    {
      return rc;
    }
  }

  fprintf(stdout, "Possible values:\n");
  fprintf(stdout, "r1 = 0 (%d), r1 = 2 (%d)\n", r1v[0], r1v[1]);
  fprintf(stdout, "r2 = 0 (%d), r2 = 1 (%d)\n", r2v[0], r2v[1]);

  return EXIT_SUCCESS;
}
