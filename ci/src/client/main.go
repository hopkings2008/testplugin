package main

import (
	"bytes"
	"crypto/md5"
	"encoding/base64"
	"fmt"
	"io/ioutil"
	"net/http"
	"time"
)

func main() {
	b, err := ioutil.ReadFile("/home/zouyu/work/mytest/pics/f3t.jpg")
	if err != nil {
		fmt.Printf("failed to read file, err: %v\n", err)
		return
	}
	h := md5.New()
	n, err := h.Write(b)
	if err != nil {
		fmt.Printf("failed to md5 write, err: %v\n", err)
		return
	}
	fmt.Printf("file len: %d\n", n)
	md5sum := h.Sum(nil)
	filemd5 := base64.StdEncoding.EncodeToString(md5sum)
	now := time.Now()
	date := fmt.Sprintf("%2d, %s %4d %02d:%02d:%02d GMT\n", now.Day(), now.Month().String(), now.Year(), now.Hour(), now.Minute(), now.Second())
	fr := bytes.NewReader(b)
	url := "http://127.0.0.1:8080/v2/img/add_image/f3t.jpg"
	client := &http.Client{}
	req, err := http.NewRequest("PUT", url, fr)
	if err != nil {
		fmt.Printf("failed to new req, err: %v\n", err)
		return
	}
	req.Header.Add("Cookie", "xxx")
	req.Header.Add("Content-MD5", filemd5)
	req.Header.Add("Content-Type", "application/octet-stream")
	req.Header.Add("Content-Length", fmt.Sprintf("%d", n))
	req.Header.Add("Date", date)
	resp, err := client.Do(req)
	if err != nil {
		fmt.Printf("failed to send req, err: %v\n", err)
		return
	}
	fmt.Printf("resp: %v\n", resp)
	defer resp.Body.Close()
	bb, err := ioutil.ReadAll(resp.Body)
	fmt.Printf("body: %s\n", string(bb))
}
