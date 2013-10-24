/*-
 * Copyright (c) 2013, Meitian Huang <_@freeaddr.info>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "parser.h"
#include "lexer.h"
#include "dbg.h"
#include <unistd.h>
#include <getopt.h>
#include "global.h"

FILE           *fout;
int             sflag = 0;
int             dflag = 0;

void
panic(void)
{
    if (fout == stdout) {
        log_info("Panic!");
    } else {
        fclose(fout);
    }

    exit(1);
}

int
main(int argc, char *argv[])
{
    int             c;
    fout = stdout;

    while ((c = getopt(argc, argv, "so:d")) != -1) {
        switch (c) {
        case 's':
            printf("Output assembly code only\n");
            sflag = 1;
            break;

        case 'o':
            fout = fopen(optarg, "w+");
            check(fout, "Cannot open the file %s for writing", optarg);
            printf("Output to %s\n", optarg);
            break;

        case 'd':
            dflag = 1;
            break;

        default:
            printf("Unknown\n");
            break;
        }
    }

    if (optind < argc) {
        do {
            FILE           *f = fopen(argv[optind], "r");
            check(f, "Cannot open the file %s", argv[optind]);
            yyrestart(f);
            yyin = f;

            do {
                yyparse();
            } while (!feof(yyin));
        } while (++optind < argc);
    } else {
        yyparse();
    }

    return 0;
error:
    return 1;
}
