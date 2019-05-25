#!/usr/bin/perl -w

use strict;
use CGI qw(:standard);

# The relative path to the registration form.
my $INPUT_FORM = "../db.html";
# The name of the database file.
my $DATA_FILE = "db.dat";
# The delimiter used to seperate the fields in each record.
my $DELIMITER = ";";

# Variables to hold the submitted data.
my $first_name = param("firstname");
my $last_name = param("lastname");
my $address = param("address");

print header(); 
print start_html("Registration Acknowledgment");
print "<div style='padding: 15px; line-height: 150%'> \n";

# If either last name or first name is empty, prints error message and abort.
if (($last_name =~ /^$/) || ($first_name =~ /^$/)) {  # ^$ stands for empty string
  print h4("Invalid inputs. Registration cannot be accepted!");
}
# The input last name and first name are not empty. It's OK to proceed.
else {

  print_greeting();
  print "<hr size='1'><br> \n";

  # Creates the database file if it does not exist already.
  touch_db();

  if (check_duplicated_record()) {
    print h3("You have already registered! Duplicated registration will not be accepted.");
  } else {
    add_record();
    print h3("Thank you for your registration! Your information has been added in our database.");
  }
}

print_footer();
print "</div> \n";
print end_html();

# Creates the database file if it does not exist. Works just like the touch command.
sub touch_db() {
  open (FILE_HANDLE, ">>$DATA_FILE") or die "Error: $!";
  close FILE_HANDLE;
}

# Returns 1 if the same record already exists in the database. Otherwise return 0.
sub check_duplicated_record() {
  # The flag to indicate whether there is a match in the input and the existing records.
  my $matched = 0;
  # Opens the database file for reading.
  open (FILE_HANDLE, "$DATA_FILE") or die "Error: $!";
  # Reads the database file line by line until the end of file is reached.
  while (<FILE_HANDLE>) {
    # The pattern to be matched: LAST_NAME;FIRST_NAME;
    my $uc_pattern = uc($last_name) . $DELIMITER . uc($first_name) . $DELIMITER;
    # Convert the line read into upper case to be matched against the pattern.
    my $uc_line = uc($_);
    # If the input matches an existing record, updates the flag and stops reading the file.
    if ($uc_line =~ /$uc_pattern/) {
      $matched = 1;
      last;
    }
  }
  close FILE_HANDLE;
  return $matched;
}

sub add_record() {
  my $record = $last_name . $DELIMITER . $first_name . $DELIMITER . $address . "\n";
  open (FILE_HANDLE, ">>$DATA_FILE") or die "Error: $!";
  print FILE_HANDLE $record;
  close FILE_HANDLE;
}

sub print_greeting() {
  print "Welcome, ";
  print "<span style='color: brown; font-weight: bold'>$first_name $last_name</span>!<br> \n";
  print "Your address is ";
  print "<span style='color: steelblue'>$address</span>.<br> \n";
}

sub print_footer() {
  print "<p style='font-size: 12px'>";
  print "&gt; <a href='${INPUT_FORM}'>Back to Registration Form</a>";
  print "</p>";
}
