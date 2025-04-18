.data
pre_txt:	.asciiz "(process id="
post_txt:	.asciiz "): "
newline: 	.asciiz "\n"


# Going to create some variable to easier understand quanta for processes
PRIORITY_0_QUANTA: .word 1   # Lowest priority (1 quantum)
PRIORITY_1_QUANTA: .word 2
PRIORITY_2_QUANTA: .word 3
PRIORITY_3_QUANTA: .word 4
PRIORITY_4_QUANTA: .word 5   # Highest priority (5 quanta)

.text
# This is the entry point of the program, 
# responsible for registering processes and starting the scheduler.

main:
    # Register process 1 with PRIORITY 2
    la $a0, p1                # $a0 = start address of process
    lw $a1, PRIORITY_2_QUANTA # $a1 = 3 (priority level 2)
    li $v0, 100
    syscall

    # Register process 2 with PRIORITY 1 
    la $a0, p2
    lw $a1, PRIORITY_1_QUANTA # $a1 = 2
    li $v0, 100
    syscall

    # Register process 2 again with PRIORITY 4 
    la $a0, p2
    lw $a1, PRIORITY_4_QUANTA # $a1 = 5
    li $v0, 100
    syscall

# This block defines the first process, which continuously prints its process ID and a counter value.
p1:	
	li $v0, 102		# get process id and store in $t1
	syscall
	add $t1, $zero, $v0
	li $t0, 0		# $t0 as loop counter, start at 0
p1_loop:	
	la $a0, pre_txt		# print preamble text
	li $v0, 4
	syscall	
	add $a0,$zero,$t1	# print process id
	li $v0, 1
	syscall
	la $a0, post_txt	# print closing bracket 
	li $v0, 4
	syscall	
	add $a0,$zero,$t0	# print counter
	li $v0, 1
	syscall
	la $a0, newline		# print a newline
	li $v0, 4
	syscall
	addi $t0, $t0, 1	# increment counter 
	b p1_loop		# run again

# This block defines the second process, which behaves similarly to p1 
# BUT starts its counter at 1000 and increments by 5.
p2:	
	li $v0, 102		# get process id and store in $t1
	syscall
	add $t1, $zero, $v0
	li $t0, 1000		# $t0 as loop counter, start at 0
p2_loop:	
	la $a0, pre_txt		# print preamble text
	li $v0, 4
	syscall	
	add $a0,$zero,$t1	# print process id
	li $v0, 1
	syscall
	la $a0, post_txt	# print closing bracket 
	li $v0, 4
	syscall	
	add $a0,$zero,$t0	# print counter
	li $v0, 1
	syscall
	la $a0, newline		# print a newline
	li $v0, 4
	syscall
	addi $t0, $t0, 5	# increment counter 
	b p2_loop		# run again

	
.kdata
		# Need to add space for priority - which now means Priority is at offset 20
pcb:	.word  0 : 30 		# each Process Control Block (PCB) has PC, a0, v0, t0, t1, PRIOIRITY
  
                      		# we assume we can have room for 5 processes                      
curpcb:	.word 0			# offset to the current PCB (multiple of 20)
				# can be used as process_id too

.ktext	0x80000180
	mfc0 $k0, $13           # move cause from coproc0 reg $13 to $k0
	srl $k0, $k0, 2         # shift right by 2
	andi $k0, $k0, 0x1f	# cause is encoded in 5 bit
	beqz $k0, int_hdlr	# cause is hardware interrupt (encoded as 0)
	beq  $k0, 8, sys_hdlr	# cause is a syscall (encoded as 8)
	li $v0, 10		# if it is anything else we terminate (should not happen)
	syscall
	
# Here is the system call handler
sys_hdlr:
	beq $v0, 100, sys_100	# syscall 100 (register a process)
	beq $v0, 101, sys_101	# syscall 101 (start processes)
	beq $v0, 102, sys_102	# syscall 102 (return process id)
	li $v0, 10		# if it is another syscall we terminate (should not happen)
	syscall

# This system call registers a new process by searching for an available PCB slot and storing the process start address.

sys_100: #register a new process with the OS (This needs to be updated somewhere for priority)
	li    $k0, 0	  	# loop counter for pcb
s100_next:	
	lw    $k1, pcb + 0($k0) # get PC stored in pcb
	beqz  $k1, s100_alloc   # if pcb entry free, use it (Note, follow this in to pick up new priority
	addiu $k0,$k0,20        # if not free, try next pcb
	bne   $k0,100, s100_next# we can have up to 5 pcb (5 times 20byte)
	li    $v0, 10		# if it is no free pcb we terminate
	syscall
s100_alloc:
	sw    $a0, pcb + 0($k0)	# store PC in pcb (initially, start of process)
	sw    $a1, pcb + 20($k0)   # THIS IS WHERE I inject the priority - store priority at offset 20

	mfc0  $k0, $14    	# move EPC to $k0
	addiu $k0, $k0, 4 	# skip one instruction, avoid same syscall again
	mtc0  $k0, $14    	# write it back to EPC
	eret			# return to process registering this new process
	
sys_101: #start scheduling of OS processes 
	lw    $k1, pcb 		# load address of first process in pcb
	bnez  $k1, s101_valid	# check if the first one is valid (we need at least one!)
	li    $v0, 10		# if it is no valid pcb we terminate
	syscall
s101_valid: #we have a valid pcb
	mtc0  $k1, $14		# copy start PC of process into EPC
	la    $k0, 0xFFFF0013	# enable clock of Digial Lab Sim
	li    $k1, 1
	sb    $k1, ($k0)
	eret			# switch to the process now in EPC
	 	
sys_102: #return process id
	lw    $k0, curpcb 	# load current pcb offset
	add   $v0, $zero, $k0   # copy current pcb (process id) to $v0
	mfc0  $k0, $14    	# move EPC to $k0
	addiu $k0, $k0, 4 	# skip one instruction, avoid same syscall again
	mtc0  $k0, $14    	# write it back to EPC
	eret			# return to process registering this new process


# Inside in this interput handler I neet to figure out how to handle quanta.....

int_hdlr: #clock used to switch to next process (we do not check exact interrupt)
        #save the current process state in pcb
	lw    $k1, curpcb   	# load the offest to current pcb (in multiples of 20)
	la    $k0, pcb	 	# load start address of the pcb array
	add   $k0, $k0, $k1 	# get address of the current active pcb
	mfc0  $k1, $14     	# get the current program counter
	sw    $k1, 0($k0)  	# save program counter so process can resume later
	sw    $a0, 4($k0)  	# save all other relevant process state in pcb
	sw    $v0, 8($k0)
	sw    $t0, 12($k0)
	sw    $t1, 16($k0)	
	#load the next process state from pcb
	lw    $k0, curpcb 	# load current pcb offset
int_next:
	addiu $k0, $k0, 20	# add 20 to get to next pcb
	blt   $k0, 100, int_skip# go to 0 if we wrap around the pcb array
	addu  $k0, $zero, $zero
int_skip:
	lw    $k1, pcb + 0($k0) # get the stored PC from the next pcb
	bnez  $k1, int_restore  # if that PC is not 0 we can switch to this process
	b     int_next 		# go back and fetch the next pcb, this one was invalid
int_restore:
        sw    $k0, curpcb	# save the offset for the pcb we switch to
        lw    $k1, pcb + 0($k0) # load PC from that pcb
        mtc0  $k1, $14		# put PC in EPC
        lw    $a0, pcb + 4($k0) # restore all registers (restore process state)
        lw    $v0, pcb + 8($k0)
        lw    $t0, pcb + 12($k0)
        lw    $t1, pcb + 16($k0)
	eret			# continue execution of the stored process

