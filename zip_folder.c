#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zip.h>
#include <dirent.h> // ディレクトリ操作用
#include <sys/stat.h> // ファイル/ディレクトリの判別用

// ディレクトリを再帰的にZIPへ追加する関数
void add_directory_to_zip(zip_t *archive, const char *dir_path, const char *zip_path_prefix) {
    DIR *dir = opendir(dir_path);
    if (!dir) {
        perror("opendir");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        // '.' と '..' は無視する
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        // ファイル/フォルダのフルパスを作成
        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, entry->d_name);
        
        // ZIPアーカイブ内でのパスを作成
        char zip_path[1024];
        snprintf(zip_path, sizeof(zip_path), "%s%s", zip_path_prefix, entry->d_name);

        struct stat st;
        if (stat(full_path, &st) != 0) {
            perror("stat");
            continue;
        }

        // エントリがディレクトリの場合
        if (S_ISDIR(st.st_mode)) {
            // ZIP内にディレクトリを追加
            if (zip_dir_add(archive, zip_path, ZIP_FL_ENC_UTF_8) < 0) {
                fprintf(stderr, "エラー: ディレクトリ '%s' をZIPに追加できませんでした: %s\n", zip_path, zip_strerror(archive));
            } else {
                 printf(" -> ディレクトリ '%s' を追加しました。\n", zip_path);
            }
            // 再帰的にこの関数を呼び出す
            char new_prefix[1024];
            snprintf(new_prefix, sizeof(new_prefix), "%s/", zip_path);
            add_directory_to_zip(archive, full_path, new_prefix);
        } 
        // エントリがファイルの場合
        else if (S_ISREG(st.st_mode)) {
            zip_source_t *source = zip_source_file(archive, full_path, 0, 0);
            if (source == NULL) {
                fprintf(stderr, "エラー: ファイル '%s' を読み込めませんでした: %s\n", full_path, zip_strerror(archive));
                continue;
            }

            if (zip_file_add(archive, zip_path, source, ZIP_FL_ENC_UTF_8) < 0) {
                fprintf(stderr, "エラー: ファイル '%s' をZIPに追加できませんでした: %s\n", zip_path, zip_strerror(archive));
                zip_source_free(source);
            } else {
                printf(" -> ファイル '%s' を追加しました。\n", zip_path);
            }
        }
    }

    closedir(dir);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "使い方: %s <出力するZIPファイル名> <圧縮するフォルダ名>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *zip_filename = argv[1];
    const char *source_dir = argv[2];
    int error = 0;

    zip_t *archive = zip_open(zip_filename, ZIP_CREATE | ZIP_TRUNCATE, &error);
    if (archive == NULL) {
        zip_error_t zip_err;
        zip_error_init_with_code(&zip_err, error);
        fprintf(stderr, "エラー: ZIPファイル '%s' を開けませんでした: %s\n", zip_filename, zip_error_strerror(&zip_err));
        zip_error_fini(&zip_err);
        return EXIT_FAILURE;
    }

    printf("'%s' をZIPファイル '%s' に圧縮中...\n", source_dir, zip_filename);
    
    // ディレクトリ圧縮処理を呼び出す (ZIP内のプレフィックスは空文字列)
    add_directory_to_zip(archive, source_dir, "");

    if (zip_close(archive) < 0) {
        fprintf(stderr, "エラー: ZIPファイル '%s' を閉じられませんでした: %s\n", zip_filename, zip_strerror(archive));
        return EXIT_FAILURE;
    }

    printf("正常にZIPファイルを作成しました。\n");

    return EXIT_SUCCESS;
}