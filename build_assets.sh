#!/bin/bash
# build_assets: Copies files from a source path to an output directory, as controlled by a manifest
#
# The manifest contains 0 or more lines, each of the form sourcePattern:outputPath (where
#  sourcePattern uses standard shell globbing operators). All files which match the pattern will
#  be copied into the output path. The pattern is relative to sourceDir and the the output
#  is relative to out_dir.
#  A line prefixed with a '-' will remove the matching files from the specified directory
#  A line prefixed with '#' is ignored as a comment

build_dir="$(dirname "${BASH_SOURCE[0]}")"
source "$build_dir/setenv.sh"

set -$-ue${DEBUG+xv}

# This function copies the files from the source directory
# to the output directory, as defined by the manifest
# Arguments:
# 1: source base directory
# 2: output base directory
# 3: path to the manifest
function perform_copy()
{
    local manifest=$1
    local src_dir=$2
    local out_dir=$3

    echo >&2 "[INFO] Processing ${manifest}"
    rm -rf "${out_dir}"
    mkdir "${out_dir}"

    # Read the manifest line by line
    while IFS=$'\r' read -r line || [[ -n "${line}" ]]
    do
        # Skip empty and commented lines
        if [[ -n "${line}" ]] && [[ "${line}" != \#* ]]; then
            # Parse format: "${sourcePattern}:${destPath}"
            IFS=': ' read -r sourcePattern destPath <<<"$line"

            # Check if sourcePattern starts with "-":
            # "-xxx/yyy" removes ${out_dir}/xxx/yyy
            # "-*/yyy" removes ${out_dir}/*/yyy by wildcard
            if [[ "${sourcePattern}" == -* ]]; then
                # Remove leading "-" from sourcePattern
                sourcePattern=${sourcePattern#-}
                # Check if sourcePattern contains "*" wildcard
                if [[ "${sourcePattern}" =~ '*' ]]; then
                    echo >&2 "[INFO] Remove ${sourcePattern}"
                    eval rm -rf "${out_dir}"/${sourcePattern}
                else
                    # Check that sourcePattern exists in the output directory
                    if [[ -e "${out_dir}/${sourcePattern}" ]]; then
                        echo >&2 "[INFO] Remove ${sourcePattern}"
                        rm -rf "${out_dir}/${sourcePattern}"
                    else
                        echo >&2 "[ERROR] ${sourcePattern} doesn't exist, fix the asset manifest"; exit 1;
                    fi
                fi
            elif [[ -d "${src_dir}/${sourcePattern}" ]]; then
                # Copy directory recursively
                echo >&2 "[INFO] Copy ${sourcePattern}"
                # destPath is optional, use sourcePattern if not set
                if [[ -n "${destPath}" ]]; then
                    local dest_dir="${out_dir}/${destPath}"
                else
                    local dest_dir="${out_dir}/${sourcePattern}"
                fi
                mkdir -p "${dest_dir}"
                cp -r "${src_dir}/${sourcePattern}"/* "${dest_dir}"
            elif [[ -e "${src_dir}/${sourcePattern}" ]]; then
                # Copy single file
                echo >&2 "[INFO] Copy ${sourcePattern}"
                # destPath is optional, use sourcePattern if not set
                if [[ -n "${destPath}" ]]; then
                    local dest_file="${out_dir}/${destPath}"
                else
                    local dest_file="${out_dir}/${sourcePattern}"
                fi
                mkdir -p "$(dirname "${dest_file}")"
                cp -r "${src_dir}/${sourcePattern}" "${dest_file}"
            elif [[ "${sourcePattern}" =~ '*' ]]; then
                # Copy files by wildcard pattern
                echo >&2 "[INFO] Copy ${sourcePattern}"
                # destPath is optional, use sourcePattern if not set
                if [[ -n "${destPath}" ]]; then
                    local dest_dir="${out_dir}/${destPath}"
                else
                    local dest_dir="${out_dir}/$(dirname "${sourcePattern}")"
                fi
                mkdir -p "${dest_dir}"
                eval cp -r "${src_dir}"/${sourcePattern} "${dest_dir}"
            elif [[ "${sourcePattern}" == "docs" ]]; then
                # Ignore missing doxygen documentation (not needed by Mbed assets)
                echo >&2 "[INFO] ${sourcePattern} doesn't exist, skipped doxygen html documentation"
            else
                echo >&2 "[ERROR] ${sourcePattern} doesn't exist, fix the asset manifest"; exit 1;
            fi
        fi
    done < "${manifest}"
}

# This function creates the asset ZIP archive, with version.xml
# Arguments:
# 1: directory to package into the archive (output/bsp_csp)
# 2: path to the ZIP archive to save (output/bsp_csp-1.0.0.10.zip)
# 3: package version (1.0.0.10) - optional, to generate version.xml
function create_zip()
{
    local pkg_dir=$1
    local pkg_zip=$2
    local pkg_ver=${3:-}

    # Remove existing asset ZIP package
    rm -rf "$pkg_zip"

    # Create asset ZIP package
    echo >&2 "[INFO] Create ${pkg_zip}"
    [[ -n "${pkg_ver}" ]] && echo "<version>${pkg_ver}</version>" > "${pkg_dir}/version.xml"
    (cd "$(dirname "${pkg_dir}")" && zip -rqX "${pkg_zip}" "$(basename "${pkg_dir}")")
}

function main()
{
    # Create CSP asset at output/psoc6csp, package as output/psoc6csp-XXXX.zip
    perform_copy "$SCRIPTS_DIR/psoc6csp.txt" "$ROOT_DIR" "$OUT_DIR/psoc6csp"
    create_zip "$OUT_DIR/psoc6csp" "$OUT_DIR/psoc6csp-$PKG_VERSION.zip" "$PKG_VERSION"

    # Package BSP assets as output/bsp/$bsp-XXXX.zip
    # Note: BSP directories are not copied to output/bsp; packaged directly from git tree
    mkdir -p "$OUT_DIR/bsp"
    for bsp_dir in "$ROOT_DIR"/bsp/*; do
        [[ -d "$bsp_dir" ]] || continue # skip non-directories
        local bsp="$(basename $bsp_dir)"
        create_zip "$bsp_dir" "$OUT_DIR/bsp/$bsp-$PKG_VERSION.zip" "$PKG_VERSION"
    done

    # Package make asset as output/make-XXXX.zip
    create_zip "$ROOT_DIR/mk_build_infra/modus-build/psoc6/make" "$OUT_DIR/make-$PKG_VERSION.zip" "$PKG_VERSION"

    # Package sdk/make asset as output/tools-make-XXXX.zip (do not include version.xml)
    create_zip "$ROOT_DIR/mk_build_infra/modus-build/tools/make" "$OUT_DIR/tools-make-$PKG_VERSION.zip"
}

# Only execute main function when not sourced by another script
if [[ "$0" == "$BASH_SOURCE" ]]; then
    main $@
fi
