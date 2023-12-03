#pragma once

#include <execinfo.h>
#include <signal.h>
#include <cxxabi.h> // demangle
#include <iostream>

/// add link option ADD_LINK_OPTIONS(-rdynamic)  to cmake file
namespace cyniu
{

  namespace BACKTRACE
  {
    void handleCrash(int sig)
    {

      void *array[100];

      // Signal catched
      fprintf(stderr, "handleCrash() signal received is: %d\n", sig);

      // Backtrace available
      int n_addresses = backtrace(array, 100);
      fprintf(stderr, "backtrace() returned addresses: %d\n", n_addresses);

      // Instead of 'backtrace_symbols_fd(array, n_addresses, STDERR_FILENO)', we will iterate to demangle symbols:
      char **addresses = backtrace_symbols(array, n_addresses);

      if (addresses == NULL)
      {
        perror("backtrace_symbols");
        exit(EXIT_FAILURE);
      }

      // Demangle backtrace addresses:
      std::string address, symbol;
      // This check is removed.
      // cppcheck-suppress variableScope
      size_t openPos, closePos, plusPos;
      char *demangledName;
      int status;

      for (int k = 0; k < n_addresses; k++)
      {
        // Address example: ./a.out(_Z16displayBacktracev) [0x556df272d3f6]
        // Need to extract the symbol within brackets:
        address = addresses[k];
        openPos = address.find("(") + 1;
        closePos = address.find(")");

        if (closePos > openPos)
        {
          symbol = address.substr(openPos, closePos - openPos);
          plusPos = symbol.find("+");

          if (plusPos > 1)
          {
            symbol = symbol.substr(0, plusPos);
          }

          status = -1;
          demangledName = abi::__cxa_demangle(symbol.c_str(), NULL, NULL, &status);

          if (status == 0)
          {
            fprintf(stderr, "%s\n", demangledName);
          }
          else
          {
            fprintf(stderr, "%s\n", address.c_str());
          }

          free(demangledName);
        }
        else
        {
          fprintf(stderr, "%s\n", address.c_str());
        }
      }

      free(addresses);

      exit(1);
    }
  } // BACKTRACE
} // cyniu

