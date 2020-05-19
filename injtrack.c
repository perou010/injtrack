#include <string.h>
#include <stdio.h>

/*
 * @param [order number]
 * @param [PO number]
 * @param [1st tracking number]
 * ...
 * @param [nth tracking number]
 */
int main(int argc, char *argv[]) {

	if (argc < 4) {
		printf("Expected at least 3 arguments. Exiting...\n");
		return 1;
	}

	FILE* template = fopen("template.html", "r");
	FILE* order = fopen("order.html", "w");

	char line_buffer[150];
	char* carrier;

	char* tracking_line_template;

	/* 
	 * Carrier determination logic
	 * TOOO can a shipment be split across carriers??
	 */	
	if((*(argv+3))[0] == '1' && (*(argv+3))[1] == 'Z') {
		carrier = "UPS";
		tracking_line_template = "\t\t<li><a href=\"https://www.ups.com/mobile/track?trackingNumber=%s\">%s</a></li>\n";
	} else {
		carrier = "FEDEX";
		tracking_line_template = "\t\t<li><a href=\"https://www.fedex.com/apps/fedextrack/?action=track&tracknumbers=%s\">%s</a></li>\n";
	}

	char* header_line_template = "\t<p>The following %s tracking %s for Order %s, PO %s</p>"; 

	/*
	 * Copy template + inject data
	 */
	while(fgets(line_buffer, 150, template) != NULL) {
		fputs(line_buffer, order);
		if(!strcmp(line_buffer, "<!--header-->\n")) {
			char* sing = "number is";
			char* plur = "numbers are";
			fprintf(order, header_line_template, carrier, (argc > 4) ? plur : sing, *(argv+1), *(argv+2));
		} else if(!strcmp(line_buffer, "<!--tracking_list-->\n")) {
			for(char **pargv = argv+3; *pargv != NULL; pargv++) fprintf(order, tracking_line_template, *pargv, *pargv);
		}
	}

	fclose(order);
	fclose(template);
}
