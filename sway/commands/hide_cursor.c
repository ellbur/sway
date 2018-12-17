#define _POSIX_C_SOURCE 200809L
#include <string.h>
#include "sway/commands.h"
#include "sway/config.h"
#include "sway/input/cursor.h"
#include "sway/input/seat.h"
#include "stringop.h"

struct cmd_results *cmd_hide_cursor(int argc, char **argv) {
	struct cmd_results *error = NULL;
	if ((error = checkarg(argc, "hide_cursor", EXPECTED_EQUAL_TO, 1))) {
		return error;
	}

	char *end;
	int timeout = strtol(argv[0], &end, 10);
	if (*end) {
		return cmd_results_new(CMD_INVALID, "hide_cursor",
				"Expected an integer timeout");
	}
	if (timeout < 100 && timeout != 0) {
		timeout = 100;
	}
	config->hide_cursor_timeout = timeout;

	struct sway_seat *seat;
	wl_list_for_each(seat, &server.input->seats, link) {
		wl_event_source_timer_update(seat->cursor->hide_source,
				config->hide_cursor_timeout);
	}

	return cmd_results_new(CMD_SUCCESS, NULL, NULL);
}
