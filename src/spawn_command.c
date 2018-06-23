#include"fjorge.h"
#include"strglob.h"

extern char **environ;

#define POSIX_SPAWN_USEVFORK 0x40

int spawn_command(const char **av) {
  assert(av);

  posix_spawnattr_t spflags;
 
  posix_spawnattr_init(&spflags);
  posix_spawnattr_setflags(&spflags, POSIX_SPAWN_USEVFORK);

  auto pid_t apid = 0;

  fjprintf_debug("Spawning command: %s", *av);

  const int sret = posix_spawn(&apid, *av, NULL, &spflags, (char *const *)av, environ);

  if(sret) {
    fjprintf_error("posix_spawn: %s", strerror(errno));

    return sret;
  }

  posix_spawnattr_destroy(&spflags);

  fjprintf_debug("Child PID: %i", apid);

  int status = 0;

  do {
    const int s = waitpid(apid, &status, 0);

    if(s == -1) {
      perror("waitpid");

      exit(EXIT_FAILURE);
    } else 
      fjprintf_verbose("Child exited with status %i\n", status);

    if(WIFEXITED(status))
      fjprintf_debug("%s[%d]: exited, status=%d\n", *av, apid, WEXITSTATUS(status));
    else if (WIFSIGNALED(status))
      fjprintf_debug("%s[%d]: killed by signal %d\n", *av, apid, WTERMSIG(status));
    else if (WIFSTOPPED(status))
      fjprintf_debug("%s[%d]: stopped by signal %d\n", *av, apid, WSTOPSIG(status));
    else if (WIFCONTINUED(status)) 
      fjprintf_debug("%s[%d]: continued\n", *av, apid);
  } while(!WIFEXITED(status) && !WIFSIGNALED(status));

  exit(EXIT_SUCCESS);

  return status;
}
