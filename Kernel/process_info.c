#include "process_info.h"
#include "strings.h"

int get_process_info_by_pid(process_info * pi, uint64_t pid) {
	process * p = get_process_by_pid(pid);
	if (p == NULL)
		return 0;
	get_process_info(pi, p);
	return 1;
}

void get_process_info (process_info * pi, process * p) {
	assign_quantum();
	
	pi->is_foreground = get_foreground_process() == p;

	if (p == get_current_process())
		pi->st = RUNNING;
	else
		pi->st = is_blocked_process(p) ? BLOCKED : READY;

	pi->pid = pid_process(p);
	pi->ppid = ppid_process(p);

	get_name_process(pi->name, p);

	pi->stack_address = stack_page_process(p);
	data_pages_process(p, pi->mem_pages);
	
	unassign_quantum();
}