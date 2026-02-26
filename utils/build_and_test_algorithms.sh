#!/bin/bash
# SPDX-License-Identifier: BSD-3-Clause

set -e

cd "$(realpath "$(dirname "$0")/..")"

error() {
    echo "$1"
    exit 1
}

# Fără COMPILE_ONLY: rulează și testele, ca să putem verifica "Problema task-x completa."
declare -a labs=(
    "lab01"
    "lab02"
    "lab03"
    "lab04"
    "lab05"
    "lab06"
    "lab07"
    "lab08"
    "lab09"
    "lab10"
    "lab11"
)

TARGET_DIR="algorithms"
for LAB in "${labs[@]}"; do
    pushd "${TARGET_DIR}/${LAB}" 2>&1 1>/dev/null

    # Cpp: pentru fiecare task, rulează check cu fiecare .cpp (în afară de main.cpp)
    for taskdir in cpp/task-*; do
        [ -d "$taskdir" ] || continue
        taskname="$(basename "$taskdir")"
        for sol in "${taskdir}"/*.cpp; do
            [ -f "$sol" ] || continue
            [ "$(basename "$sol")" = "main.cpp" ] && continue
            solname="$(basename "$sol")"
            solpath="${TARGET_DIR}/${LAB}/${taskdir}/${solname}"
            echo ""
            echo "================================================"
            echo "Testing ${solpath} ..."
            tmpout=$(mktemp)
            set +e
            ./check cpp "$taskname" "$solname" > "$tmpout" 2>&1
            exitcode=$?
            set -e
            cat "$tmpout"
            pass=0
            if [[ $exitcode -eq 0 ]] && grep -q "Problema ${taskname} completa." "$tmpout"; then
                pass=1
            fi
            rm -f "$tmpout"
            rm -f "${taskdir}/${solname%.cpp}"
            if [[ $pass -ne 1 ]]; then
                echo "Nota: In .err.log se afla si afisarile la stdout si stderr din implementarea voastra."
                error "${solpath}: check failed (exit $exitcode or missing 'Problema ${taskname} completa.')"
            fi
            echo "-> check passed"
        done
    done

    # Java: pentru fiecare task, rulează check cu fiecare .java din src/ (în afară de Main.java)
    for taskdir in java/task-*; do
        [ -d "$taskdir" ] || continue
        taskname="$(basename "$taskdir")"
        srcdir="${taskdir}/src"
        [ -d "$srcdir" ] || continue
        for sol in "${srcdir}"/*.java; do
            [ -f "$sol" ] || continue
            [ "$(basename "$sol")" = "Main.java" ] && continue
            solname="$(basename "$sol")"
            solpath="${TARGET_DIR}/${LAB}/${srcdir}/${solname}"
            echo ""
            echo "================================================"
            echo "Testing ${solpath} ..."
            tmpout=$(mktemp)
            set +e
            ./check java "$taskname" "$solname" > "$tmpout" 2>&1
            exitcode=$?
            set -e
            cat "$tmpout"
            pass=0
            if [[ $exitcode -eq 0 ]] && grep -q "Problema ${taskname} completa." "$tmpout"; then
                pass=1
            fi
            rm -f "$tmpout"
            rm -rf "${taskdir}/bin"
            if [[ $pass -ne 1 ]]; then
                echo "Nota: In .err.log se afla si afisarile la stdout si stderr din implementarea voastra."
                error "${solpath}: check failed (exit $exitcode or missing 'Problema ${taskname} completa.')"
            fi
            echo "-> check passed"
        done
    done

    popd 2>&1 1>/dev/null
done

echo "${TARGET_DIR}/* compiled!"

export COMPILE_ONLY=
