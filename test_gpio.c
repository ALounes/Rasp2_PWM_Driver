#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>

// Commands for the PWM management in the kernel module
#define COMMAND_PWM_ENABLE_DISABLE 30
#define COMMAND_DUTY_CONFIG 31
#define COMMAND_PERIOD_CONFIG 32

/* Test file to check the functionalites of the associated kernel module:
	The character device is opened as a file, and we send data to it. */

int main(void) {
	unsigned int cmd;
	unsigned long arg;

	int fd = open("/dev/gpio_pwm_module", O_RDWR);
	if (fd == -1) {
		printf("Error when opening /dev/gpio_pwm_module\n");
		exit(1);
	}	

	while(1) {
		printf("Enter a command:\n - handle the PWM (0 to enable/disable it, 1 to configure the duty cycle and the period)\n - your GPIO port number (must be between 2 and 27)\n - set the value of the GPIO ports to 0 (99) or to 1 (100)\n-> ");
		scanf("%d", &cmd);

		switch (cmd) {

			case 0:
				/*	ioctl(int fd, int cmd, ...):
						fd: kernel module to access
						cmd: enable/disable the PWM device (30)
						additional argument: set to 0, because not used */

				if (ioctl(fd, COMMAND_PWM_ENABLE_DISABLE, 0)) {
					printf("Error when enabling/disabling the PWM\n");
					exit(1);
				}
				break;

			case 1:
				printf("Enter the duty cycle (in nanoseconds) -> ");
				scanf("%lu", &arg);

				/*	ioctl(int fd, int cmd, ...):
						fd: kernel module to access
						cmd: configure the PWM device (duty cycle) (31)
						additional argument: the duty cycle to configure */

				if (ioctl(fd, COMMAND_DUTY_CONFIG, arg)) {
					printf("Error when configuring the duty cycle for the PWM\n");
					exit(1);
				}

				printf("Enter the period (in nanoseconds) -> ");
				scanf("%lu", &arg);

				/*	ioctl(int fd, int cmd, ...):
						fd: kernel module to access
						cmd: configure the PWM device (period) (32)
						additional argument: the period to configure */

				if (ioctl(fd, COMMAND_PERIOD_CONFIG, arg)) {
					printf("Error when configuring the period for the PWM\n");
					exit(1);
				}
				break;

			case 2:
			case 3:
			case 4:
			case 14:
			case 15:
			case 17:
			case 27:
			case 22:
			case 23:
			case 24:
			case 10:
			case 9:
			case 11:
			case 25:
			case 8:
			case 7:
			case 5:
			case 6:
			case 13:
			case 19:
			case 26:
			case 12:
			case 16:
			case 20:
			case 21:
				printf("Enter your argument for your GPIO port:\n0 to switch it off, 1 to switch it on, 2 to define it as input, 3 to read its value\n-> ");
				scanf("%lu", &arg);

				/*	ioctl(int fd, int cmd, ...):
						fd: kernel module to access
						cmd: GPIO port number to handle
						additional argument: value to handle a GPIO port */

				if (ioctl(fd, cmd, arg)) {
					printf("Error when accessing the GPIO port %d\n", cmd);
					exit(1);
				}
				break;

			case 99:
			case 100:
				/*	ioctl(int fd, int cmd, ...):
						fd: kernel module to access
						cmd: * 99: set the value of the GPIO ports to 0
							  * 100: set the value of the GPIO ports to 1
						additional argument: set to 0, because not used */

				if (ioctl(fd, cmd, 0)) {
					printf("Error when enabling/disabling the PWM\n");
					exit(1);
				}
				break;

			default:
				printf("Unknown command (%d)\n", cmd);
				exit(1);
		}

		printf("------\n");
	}

	close(fd);
	return 0;
}
