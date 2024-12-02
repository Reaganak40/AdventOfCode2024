def is_safe(report):
    sorted_report = sorted(report)
    if report == sorted_report or report == sorted_report[::-1]:
        for i in range(1, len(report)):
            if abs(report[i] - report[i - 1]) > 3 or report[i] == report[i - 1]:
                return False
        return True
    return False


def is_safe_removing_1(report):
    sorted_report = sorted(report)
    if report == sorted_report or report == sorted_report[::-1]:
        for i in range(1, len(report)):
            if abs(report[i] - report[i - 1]) > 3 or report[i] == report[i - 1]:
                report_removed_i = report[:i] + report[i + 1 :]
                report_removed_i_1 = report[: i - 1] + report[i:]
                return is_safe(report_removed_i) or is_safe(report_removed_i_1)
        return True
    else:
        for i in range(0, len(report)):
            report_removed_i = report[:i] + report[i + 1 :]
            if is_safe(report_removed_i):
                return True

    return False


with open("input.txt", "r") as file:
    reports = file.read().splitlines()

reports = [list(map(int, report.split())) for report in reports]

def preport(report):
    for i in report:
        print(i, end=" ")
    print()

for report in reports:
    if is_safe_removing_1(report):
        print("Safe:", end=" ")
        preport(report)
    else:
        print("Unsafe:", end=" ")
        preport(report)