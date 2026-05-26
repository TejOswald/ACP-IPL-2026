#include <stdio.h>
#include <ctype.h>
#include <string.h>

union decision {
    unsigned char flags;
    struct {
        int your_choice:1; /* to be computed */
        int your_mothers_choice:1;
        int your_fathers_choice:1;
        int socially_acceptable:1;
        int financially_viable:1;
        int do_you_aptitude:1;
        int do_you_likeit:1;
        int decision:1; /* to be computed */
    } bits;
};

static int ask_yes_no(const char *question) {
    char buffer[32];
    while (1) {
        printf("%s (y/n): ", question);
        if (!fgets(buffer, sizeof buffer, stdin)) {
            return 0;
        }
        for (char *p = buffer; *p; ++p) {
            *p = (char) tolower((unsigned char) *p);
        }
        if (strchr(buffer, 'y')) {
            return 1;
        }
        if (strchr(buffer, 'n')) {
            return 0;
        }
        printf("Please answer with y or n.\n");
    }
}

union decision input(void) {
    union decision d = {0};
    d.bits.your_mothers_choice = ask_yes_no("Does your mother think this is a good idea?");
    d.bits.your_fathers_choice = ask_yes_no("Does your father think this is a good idea?");
    d.bits.socially_acceptable = ask_yes_no("Is it socially acceptable?");
    d.bits.financially_viable = ask_yes_no("Is it financially viable?");
    d.bits.do_you_aptitude = ask_yes_no("Do you have the aptitude to do it?");
    d.bits.do_you_likeit = ask_yes_no("Do you actually like it?");
    return d;
}

void make_decision(union decision *d) {
    /*
     * This is the decision expression that matches a practical personal choice.
     * You want it to be something you like and can do, or at least something
     * that is financially viable and socially acceptable. The final decision
     * also prefers parental support.
     */
    int personal_drive = d->bits.do_you_likeit && d->bits.do_you_aptitude;
    int safe_choice = d->bits.financially_viable && d->bits.socially_acceptable;
    d->bits.your_choice = personal_drive || safe_choice;
    d->bits.decision = d->bits.your_choice &&
                       (d->bits.your_mothers_choice || d->bits.your_fathers_choice);
}

static int count_set_bits(unsigned char value) {
    int count = 0;
    while (value) {
        count += value & 1u;
        value >>= 1;
    }
    return count;
}

void print_decsion(union decision d) {
    printf("\nDecision narrative:\n");
    printf("  - Your mother\'s choice: %s\n", d.bits.your_mothers_choice ? "Yes" : "No");
    printf("  - Your father\'s choice: %s\n", d.bits.your_fathers_choice ? "Yes" : "No");
    printf("  - Socially acceptable: %s\n", d.bits.socially_acceptable ? "Yes" : "No");
    printf("  - Financially viable: %s\n", d.bits.financially_viable ? "Yes" : "No");
    printf("  - You have aptitude: %s\n", d.bits.do_you_aptitude ? "Yes" : "No");
    printf("  - You like it: %s\n", d.bits.do_you_likeit ? "Yes" : "No");
    printf("  - Your computed choice: %s\n", d.bits.your_choice ? "Yes" : "No");
    printf("  - Final decision: %s\n", d.bits.decision ? "Go for it" : "Reconsider it");
}

void print_conclusion_based_on_flags(union decision d) {
    unsigned char value = d.flags;
    int set_bits = count_set_bits(value);

    printf("\nFlags byte stored as: %u\n", value);
    printf("Interpreting the packed flags as a decision profile with %d active bit(s).\n", set_bits);

    if (value == 0) {
        printf("Conclusion: No reason to move forward, everything is off.\n");
    } else if (set_bits <= 2) {
        printf("Conclusion: Weak support. The flags suggest caution and a need for more positive signs.\n");
    } else if (set_bits <= 5) {
        printf("Conclusion: Mixed signals. Some good reasons exist, but final approval is not automatic.\n");
    } else {
        printf("Conclusion: Strong support. The flags line up well for a positive outcome.\n");
    }

    printf("(Bitwise view: mother=%d father=%d social=%d money=%d aptitude=%d like=%d choice=%d decision=%d)\n",
           d.bits.your_mothers_choice,
           d.bits.your_fathers_choice,
           d.bits.socially_acceptable,
           d.bits.financially_viable,
           d.bits.do_you_aptitude,
           d.bits.do_you_likeit,
           d.bits.your_choice ? 1 : 0,
           d.bits.decision ? 1 : 0);
}

int main(void) {
    union decision d = input();
    make_decision(&d);
    print_decsion(d);
    print_conclusion_based_on_flags(d);
    return 0;
}
