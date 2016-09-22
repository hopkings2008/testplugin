package main

import (
	"crypto/hmac"
	"crypto/sha1"
	"encoding/base64"
	"encoding/json"
	"fmt"
	"github.com/julienschmidt/httprouter"
	"io/ioutil"
	"log"
	"net/http"
	"os"
	"strings"
)

type UserInfo struct {
	Uid     int    `json:"user_id"`
	ErrCode int    `json:"err_code"`
	ErrMsg  string `json:"err_msg"`
}

func Index(w http.ResponseWriter, r *http.Request, _ httprouter.Params) {
	fmt.Fprint(w, "Welcome!\n")
}

func Hello(w http.ResponseWriter, r *http.Request, ps httprouter.Params) {
	fmt.Fprintf(w, "hello, %s!\n", ps.ByName("name"))
}

func Auth(w http.ResponseWriter, r *http.Request, _ httprouter.Params) {
	fmt.Printf("got auth req: %v\n", r)
	u := UserInfo{
		Uid:     123,
		ErrCode: 0,
		ErrMsg:  "",
	}
	b, _ := json.Marshal(u)
	fmt.Fprintf(w, "%s", string(b))
	//w.WriteHeader(http.StatusOK)
}

func Upload(w http.ResponseWriter, r *http.Request, _ httprouter.Params) {
	fmt.Printf("received req: %v\n", r)
	aks := make(map[string]string)
	aks["testak"] = "testsk"
	authstr := r.Header["Authorization"]
	if authstr == nil {
		fmt.Printf("no auth.\n")
		//w.Write([]byte("no auth"))
		w.WriteHeader(http.StatusBadRequest)
		return
	}
	strs := strings.Split(authstr[0], ":")
	if len(strs) == 1 {
		fmt.Printf("invalid auth.\n")
		//w.Write([]byte("invalid path."))
		w.WriteHeader(http.StatusBadRequest)
		return
	}
	ossstr := strs[0]
	temp := strings.Split(ossstr, " ")
	if len(temp) == 1 {
		fmt.Printf("invalid oss str.\n")
		//w.Write([]byte("invalid oss str."))
		w.WriteHeader(http.StatusBadRequest)
		return
	}
	oss := temp[1]
	if oss != "OSS" {
		fmt.Printf("no OSS.\n")
		//w.Write([]byte("no OSS"))
		w.WriteHeader(http.StatusBadRequest)
		return
	}
	ak := temp[1]
	sk, ok := aks[ak]
	if !ok {
		fmt.Printf("unknown ak.\n")
		//w.Write([]byte("unknown ak."))
		w.WriteHeader(http.StatusBadRequest)
		return
	}
	sig := strs[1]
	md5str := ""
	tt := ""
	md5s := r.Header["Content-MD5"]
	if md5s != nil {
		md5str = md5s[0]
	}
	ct := r.Header["Content-Type"]
	if ct != nil {
		tt = ct[0]
	}
	date := ""
	ds := r.Header["Date"]
	if ds != nil {
		date = ds[0]
	}
	method := r.Method
	resource := r.URL.Path
	hash := hmac.New(sha1.New, []byte(sk))
	hstr := fmt.Sprintf("%s\n%s\n%s\n%s\n%s", method, md5str, tt, date, resource)
	hash.Write([]byte(hstr))
	mac := hash.Sum(nil)
	final := base64.StdEncoding.EncodeToString(mac)
	if final != sig {
		fmt.Printf("sig: %s, final: %s\n", sig, final)
		//w.Write([]byte(fmt.Sprintf("sig: %s, final: %s", sig, final)))
		w.WriteHeader(http.StatusBadRequest)
		return
	}

	//calculte the sig from request.
	b, err := ioutil.ReadAll(r.Body)
	err = ioutil.WriteFile("./"+resource, b, os.ModePerm)
	if err != nil {
		fmt.Printf("failed to write %s, err: %v\n", resource, err)
		//w.Write([]byte(fmt.Sprintf("failed to write %s, err: %v\n", resource, err)))
		w.WriteHeader(http.StatusBadRequest)
		return
	}
	w.WriteHeader(http.StatusOK)
}

func Download(w http.ResponseWriter, r *http.Request, _ httprouter.Params) {
	fmt.Printf("got download req: %v\n", r)
	aks := make(map[string]string)
	aks["testak"] = "testsk"
	authstr := r.Header["Authorization"]
	if authstr == nil {
		fmt.Printf("no auth.\n")
		//w.Write([]byte("no auth"))
		w.WriteHeader(http.StatusBadRequest)
		return
	}
	strs := strings.Split(authstr[0], ":")
	if len(strs) == 1 {
		fmt.Printf("invalid auth.\n")
		//w.Write([]byte("invalid path."))
		w.WriteHeader(http.StatusBadRequest)
		return
	}
	ossstr := strs[0]
	temp := strings.Split(ossstr, " ")
	if len(temp) == 1 {
		fmt.Printf("invalid oss str.\n")
		//w.Write([]byte("invalid oss str."))
		w.WriteHeader(http.StatusBadRequest)
		return
	}
	oss := temp[1]
	if oss != "OSS" {
		fmt.Printf("no OSS.\n")
		//w.Write([]byte("no OSS"))
		w.WriteHeader(http.StatusBadRequest)
		return
	}
	ak := temp[1]
	sk, ok := aks[ak]
	if !ok {
		fmt.Printf("unknown ak.\n")
		//w.Write([]byte("unknown ak."))
		w.WriteHeader(http.StatusBadRequest)
		return
	}
	sig := strs[1]
	md5str := ""
	tt := ""
	md5s := r.Header["Content-MD5"]
	if md5s != nil {
		md5str = md5s[0]
	}
	ct := r.Header["Content-Type"]
	if ct != nil {
		tt = ct[0]
	}
	date := ""
	ds := r.Header["Date"]
	if ds != nil {
		date = ds[0]
	}
	method := r.Method
	resource := r.URL.Path
	hash := hmac.New(sha1.New, []byte(sk))
	hstr := fmt.Sprintf("%s\n%s\n%s\n%s\n%s", method, md5str, tt, date, resource)
	hash.Write([]byte(hstr))
	mac := hash.Sum(nil)
	final := base64.StdEncoding.EncodeToString(mac)
	if final != sig {
		fmt.Printf("sig: %s, final: %s\n", sig, final)
		//w.Write([]byte(fmt.Sprintf("sig: %s, final: %s", sig, final)))
		w.WriteHeader(http.StatusBadRequest)
		return
	}

	b, err := ioutil.ReadFile("." + resource)
	if err != nil {
		fmt.Printf("failed to read %s, err: %v\n", resource, err)
		//w.Write([]byte(fmt.Sprintf("failed to read %s, err: %v\n", resource, err)))
		w.WriteHeader(http.StatusBadRequest)
		return
	}
	w.Write(b)
	//w.WriteHeader(http.StatusOK)

}

func main() {
	router := httprouter.New()
	router.GET("/", Index)
	router.GET("/hello/:name", Hello)
	router.POST("/v1/user/authorize", Auth)
	router.PUT("/oss/upload", Upload)
	router.GET("/oss/download", Download)

	log.Fatal(http.ListenAndServe(":9091", router))
}
