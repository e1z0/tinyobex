/*
        Tiny OBEX
        OBEX PUSH over IrDA
        Copyright (c) 2023 e1z0 <e1z0@vintage2000.org>
	    Copyright (c) 2007 Christian W. Zuckschwerdt <zany@triq.net>
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <obexftp/client.h>

bool file_exists(const char *filename);


// return true if the file specified by the filename exists
bool file_exists(const char *filename) {
        FILE *fp = fopen(filename, "r");
        bool is_exist = false;
        if (fp != NULL)
        {
                is_exist = true;
                fclose(fp); // close the file
        }
        return is_exist;
}

int main(int argc, char *argv[])
{
	char *device = NULL;
	int channel = 0;
	char *filepath, *filename;
	obexftp_client_t *cli = NULL; /*!!!*/
	int ret;

	/* Get the filename, device and optional channel */
	if (argc < 2) {
                fprintf(stderr, "Tiny OBEX\nPush tool for IrDA\n");
				fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
				exit(1);
	}
	filepath = argv[1];

  	/* Extract basename from file path */
    if (!file_exists(filepath)) {
                fprintf(stderr, "File %s does not exist\n",filepath);
                exit(1);
	}

	filename = strrchr(filepath, '/');
	if (!filename)
		filename = filepath;
	else
		filename++;

	/* Open connection to IrDA */
	cli = obexftp_open(OBEX_TRANS_IRDA, NULL, NULL, NULL); /*!!!*/
	if (cli == NULL) {
		fprintf(stderr, "Error opening OBEX client\n");
		exit(1);
	}

        fprintf(stderr, "Connection to device: ok\n");

	/* Connect to remote device  */
	ret = obexftp_connect_push(cli, device, channel); /*!!!*/
	if (ret < 0) {
		fprintf(stderr, "Error connecting to OBEX device\n");
		obexftp_close(cli);
		cli = NULL;
		exit(1);
	}

        fprintf(stderr, "Connection to remote device: ok\n");


        fprintf(stderr, "Uploading...\n");
	/* Push file to remote device */
	ret = obexftp_put_file(cli, filepath, filename); /*!!!*/
	if (ret < 0) {
		fprintf(stderr, "Error putting file\n");
	}

        fprintf(stderr, "File upload: ok\n");

	/* Disconnect */
	ret = obexftp_disconnect(cli); /*!!!*/
	if (ret < 0) {
		fprintf(stderr, "Error disconnecting the client\n");
	}

	fprintf(stderr, "Disconnect from device: ok\n");

	/* Close */
	obexftp_close(cli); /*!!!*/
	cli = NULL;


	exit(0);
}

